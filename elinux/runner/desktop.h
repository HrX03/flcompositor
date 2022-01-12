#ifndef DESKTOP_
#define DESKTOP_

#include <wayland-server.h>

#include <taiwins/objects/desktop.h>

typedef struct fl_server {
	wl_display *display;
	struct wl_event_loop* loop; /**< main event loop */

	/* globals */
	struct tw_backend* backend;
	struct tw_engine* engine;
	struct tw_render_context* ctx;
	struct tw_desktop_surface_api desktop;
	struct tw_desktop_manager desktop_manager;
  //      struct tw_config config;
	//struct tw_server_output_manager *output_manager;
	//struct tw_server_input_manager *input_manager;
} fl_server;

bool init_fl_server(fl_server* server);

#endif