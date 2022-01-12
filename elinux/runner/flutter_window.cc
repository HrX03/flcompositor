// Copyright 2021 Sony Corporation. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "flutter_window.h"

#include <chrono>
#include <cmath>
#include <iostream>
#include <string.h>
#include <thread>

#include <taiwins/backend_headless.h>
#include <taiwins/engine.h>
#include <taiwins/render_context.h>
#include <taiwins/objects/logger.h>
#include <wayland-server.h>

#include "flutter/generated_plugin_registrant.h"

static void handle_pong(struct tw_desktop_surface *client, void *user_data) {}

struct tw_desktop_surface_api desktop = {
    .pong = handle_pong,
    //.surface_added = handle_surface_added,
    //.move = handle_move,
};

/*static void tw_desktop_fini(struct tw_desktop_manager *desktop)
{
	if (desktop->wl_shell_global)
		wl_global_destroy(desktop->wl_shell_global);
	desktop->wl_shell_global = NULL;
}

static void handle_display_destroy(struct wl_listener *listener, void *data)
{
	struct tw_desktop_manager *desktop =
		wl_container_of(listener, desktop, destroy_listener);

	tw_desktop_fini(desktop);
}

bool desktop_init(tw_desktop_manager *desktop,
                struct wl_display *display,
                const struct tw_desktop_surface_api *api,
                void *user_data,
                enum tw_desktop_init_option option)
{
	bool ret = true;
	if (!option)
		return false;

	desktop->display = display;
	desktop->user_data = user_data;
	memcpy(&desktop->api, api, sizeof(*api));
	if (option & TW_DESKTOP_INIT_INCLUDE_WL_SHELL)
		ret = ret && init_wl_shell(desktop);
	if (option & TW_DESKTOP_INIT_INCLUDE_XDG_SHELL_STABEL)
		ret = ret && init_xdg_shell(desktop);
	if (!ret)
		tw_desktop_fini(desktop);

	wl_list_init(&desktop->destroy_listener.link);
	desktop->destroy_listener.notify = handle_display_destroy;
	wl_display_add_destroy_listener(display, &desktop->destroy_listener);

	return ret;
}*/

bool init_fl_server(fl_server* server) {
    return tw_desktop_init(&server->desktop_manager, server->display,
	        &server->desktop, server, TW_DESKTOP_INIT_INCLUDE_ALL_SHELLS);
	        //TW_DESKTOP_INIT_INCLUDE_WL_SHELL |
	        //TW_DESKTOP_INIT_INCLUDE_XDG_SHELL_STABEL);
}

FlutterWindow::FlutterWindow(
    const flutter::FlutterViewController::ViewProperties view_properties,
    const flutter::DartProject project)
    : view_properties_(view_properties), project_(project) {}

bool FlutterWindow::OnCreate(fl_server* server) {
  flutter_view_controller_ = std::make_unique<flutter::FlutterViewController>(
      view_properties_, project_);

  // Ensure that basic setup of the controller was successful.
  if (!flutter_view_controller_->engine() ||
      !flutter_view_controller_->view()) {
    return false;
  }

  server->display = wl_display_create();
  wl_display_add_socket_auto(server->display);

  tw_logger_use_file(stdout);

  //now we create the backend and a render context
  server->backend = tw_headless_backend_create(server->display);
  const struct tw_egl_options *options =
      tw_backend_get_egl_params(server->backend);
  server->ctx = tw_render_context_create_egl(server->display, options);
  server->engine =
      tw_engine_create_global(server->display, server->backend);

  tw_backend_start(server->backend, server->ctx); //broadcast all the existing hardwares
  server->loop = wl_display_get_event_loop(server->display);
  server->desktop = desktop;

  init_fl_server(server);
  //wl_display_run(display);

  // Register Flutter plugins.
  RegisterPlugins(flutter_view_controller_->engine());

  return true;
}

void FlutterWindow::OnDestroy(fl_server* server) {
  if (flutter_view_controller_) {
    flutter_view_controller_ = nullptr;
  }

  tw_render_context_destroy(server->ctx);
  wl_display_destroy(server->display);
  tw_logger_close();
}

void FlutterWindow::Run(fl_server* server) {
  // Main loop.
  auto next_flutter_event_time =
      std::chrono::steady_clock::time_point::clock::now();
  while (flutter_view_controller_->view()->DispatchEvent()) {
    wl_display_flush_clients(server->display);
		wl_event_loop_dispatch(server->loop, 0);
    // Wait until the next event.
    {
      auto wait_duration =
          std::max(std::chrono::nanoseconds(0),
                   next_flutter_event_time -
                       std::chrono::steady_clock::time_point::clock::now());
      std::this_thread::sleep_for(
          std::chrono::duration_cast<std::chrono::milliseconds>(wait_duration));
    }

    // Processes any pending events in the Flutter engine, and returns the
    // number of nanoseconds until the next scheduled event (or max, if none).
    auto wait_duration = flutter_view_controller_->engine()->ProcessMessages();
    {
      auto next_event_time = std::chrono::steady_clock::time_point::max();
      if (wait_duration != std::chrono::nanoseconds::max()) {
        next_event_time =
            std::min(next_event_time,
                     std::chrono::steady_clock::time_point::clock::now() +
                         wait_duration);
      } else {
        // Wait for the next frame if no events.
        auto frame_rate = flutter_view_controller_->view()->GetFrameRate();
        next_event_time = std::min(
            next_event_time,
            std::chrono::steady_clock::time_point::clock::now() +
                std::chrono::milliseconds(
                    static_cast<int>(std::trunc(1000000.0 / frame_rate))));
      }
      next_flutter_event_time =
          std::max(next_flutter_event_time, next_event_time);
    }
  }
}
