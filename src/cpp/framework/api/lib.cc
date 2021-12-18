#include "framework/api/lib.h"

#include <iostream>
#include <emscripten/emscripten.h>

#include "framework/base_app.h" //

// ----------------------------------------------------------------------------

void setVerticalSync(bool status) {
  if (status) {
    emscripten_set_main_loop_timing(EM_TIMING_RAF, 1);
  } else {
    emscripten_set_main_loop_timing(EM_TIMING_SETTIMEOUT, 0);
  }
}

int getSurfaceWidth() {
  return BaseApp::GetBaseAPI().getWidth();
}

int getSurfaceHeight() {
  return BaseApp::GetBaseAPI().getHeight();
}

// ----------------------------------------------------------------------------