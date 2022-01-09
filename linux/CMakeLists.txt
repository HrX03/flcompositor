cmake_minimum_required(VERSION 3.10)
set(PROJECT_NAME "flcompositor")
project(${PROJECT_NAME} LANGUAGES CXX)

# This value is used when generating builds using this plugin, so it must
# not be changed
set(PLUGIN_NAME "flcompositor_plugin")

pkg_check_modules(DRM REQUIRED IMPORTED_TARGET libdrm)
pkg_check_modules(EVDEV REQUIRED IMPORTED_TARGET libevdev)

add_library(${PLUGIN_NAME} SHARED
  "flcompositor_plugin.cc"

  "hedgehog/backend/Backend.cpp"
  "hedgehog/backend/BackendX11Base.cpp"
  "hedgehog/backend/BackendX11EGL.cpp"
  "hedgehog/backend/BackendX11GLX.cpp"

  "hedgehog/main/main.cpp"
  "hedgehog/main/util.cpp"

  "hedgehog/opengl/RectRenderer.cpp"
  "hedgehog/opengl/ShaderProgram.cpp"
  "hedgehog/opengl/Texture.cpp"
  
  "hedgehog/scene/Scene.cpp"
  
  "hedgehog/wayland/protocols/xdg-shell-unstable-v5.c"
  "hedgehog/wayland/protocols/xdg-shell-unstable-v6.c"
  "hedgehog/wayland/Resource.cpp"
  "hedgehog/wayland/WaylandEGL.cpp"
  "hedgehog/wayland/WaylandServer.cpp"
  "hedgehog/wayland/WaylandEGL.cpp"
  "hedgehog/wayland/WlDataDeviceManager.cpp"
  "hedgehog/wayland/WlSeat.cpp"
  "hedgehog/wayland/WlShellSurface.cpp"
  "hedgehog/wayland/WlSurface.cpp"
  "hedgehog/wayland/XdgShellV6Surface.cpp"
)
apply_standard_settings(${PLUGIN_NAME})
set_target_properties(${PLUGIN_NAME} PROPERTIES
  CXX_VISIBILITY_PRESET hidden)
target_compile_definitions(${PLUGIN_NAME} PRIVATE FLUTTER_PLUGIN_IMPL)
target_include_directories(${PLUGIN_NAME} INTERFACE
  "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_include_directories(${PLUGIN_NAME} INTERFACE
  "${CMAKE_CURRENT_SOURCE_DIR}/hedgehog")

target_link_libraries(${PLUGIN_NAME} PRIVATE flutter)
target_link_libraries(${PLUGIN_NAME} PRIVATE wayland-server)
target_link_libraries(${PLUGIN_NAME} PRIVATE X11)
target_link_libraries(${PLUGIN_NAME} PRIVATE EGL)
target_link_libraries(${PLUGIN_NAME} PRIVATE GL)
target_link_libraries(${PLUGIN_NAME} PRIVATE xkbcommon-x11)
target_link_libraries(${PLUGIN_NAME} PRIVATE xkbcommon)
target_link_libraries(${PLUGIN_NAME} PRIVATE GLU)
target_link_libraries(${PLUGIN_NAME} PRIVATE GLEW)
target_link_libraries(${PLUGIN_NAME} PRIVATE SOIL)
target_link_libraries(${PLUGIN_NAME} PRIVATE gbm)
target_link_libraries(${PLUGIN_NAME} PRIVATE udev)
target_link_libraries(${PLUGIN_NAME} PRIVATE evdev)
#target_link_libraries(${PLUGIN_NAME} PRIVATE libinput)
target_link_libraries(${PLUGIN_NAME} PRIVATE drm)

target_link_libraries(${PLUGIN_NAME} PRIVATE PkgConfig::GTK)
target_link_libraries(${PLUGIN_NAME} PRIVATE PkgConfig::DRM)
target_link_libraries(${PLUGIN_NAME} PRIVATE PkgConfig::EVDEV)

# 'X11',
# 'EGL',
# 'GL',
# 'xkbcommon-x11',
# 'xkbcommon',
# 'GLU',
# 'GLEW',
# 'SOIL',
# 'gbm',
# 'udev',
# 'evdev',
# 'libinput',
# 'drm',

# List of absolute paths to libraries that should be bundled with the plugin
set(flcompositor_bundled_libraries
  ""
  PARENT_SCOPE
)
