cmake_minimum_required(VERSION 3.15)
project(runner LANGUAGES CXX)

if(FLUTTER_TARGET_BACKEND_TYPE MATCHES "gbm")
  add_definitions(-DFLUTTER_TARGET_BACKEND_GBM)
elseif(FLUTTER_TARGET_BACKEND_TYPE MATCHES "eglstream")
  add_definitions(-DFLUTTER_TARGET_BACKEND_EGLSTREAM)
elseif(FLUTTER_TARGET_BACKEND_TYPE MATCHES "x11")
  add_definitions(-DFLUTTER_TARGET_BACKEND_X11)
else()
  add_definitions(-DFLUTTER_TARGET_BACKEND_WAYLAND)
endif()

add_executable(${BINARY_NAME}
  "flutter_window.cc"
  "main.cc"
  "${FLUTTER_MANAGED_DIR}/generated_plugin_registrant.cc"
)
find_package(PkgConfig REQUIRED)
pkg_check_modules(PIXMAN REQUIRED IMPORTED_TARGET pixman-1)

apply_standard_settings(${BINARY_NAME})
target_include_directories(${BINARY_NAME} PRIVATE
  "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_link_libraries(${BINARY_NAME} PRIVATE flutter)
target_link_libraries(${BINARY_NAME} PRIVATE flutter flutter_wrapper_app)
#target_link_libraries(${BINARY_NAME} PRIVATE PkgConfig:PIXMAN)
target_link_libraries(${BINARY_NAME} PRIVATE pixman-1)
target_link_libraries(${BINARY_NAME} PRIVATE taiwins)
target_link_libraries(${BINARY_NAME} PRIVATE wayland-server)
target_link_libraries(${BINARY_NAME} PRIVATE wayland-egl)
target_include_directories(${BINARY_NAME} PRIVATE "${CMAKE_SOURCE_DIR}")
add_dependencies(${BINARY_NAME} flutter_assemble)
