//
//  Generated file. Do not edit.
//

// clang-format off

#include "generated_plugin_registrant.h"

#include <flcompositor/flcompositor_plugin.h>

void fl_register_plugins(FlPluginRegistry* registry) {
  g_autoptr(FlPluginRegistrar) flcompositor_registrar =
      fl_plugin_registry_get_registrar_for_plugin(registry, "FlcompositorPlugin");
  flcompositor_plugin_register_with_registrar(flcompositor_registrar);
}
