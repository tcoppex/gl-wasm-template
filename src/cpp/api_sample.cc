#include "cpp/api_sample.h"

#include <iostream>
#include "cpp/app.h"

// ----------------------------------------------------------------------------

void testCall() {
  std::cout << __PRETTY_FUNCTION__ << std::endl;
}

void printNumber(int i) {
  std::cout << "Number : " << i << std::endl;
}

int square(int c) {
  return c * c;
}

void setVerticalSync(bool status) {
  if (status) {
    emscripten_set_main_loop_timing(EM_TIMING_RAF, 1);
  } else {
    emscripten_set_main_loop_timing(EM_TIMING_SETTIMEOUT, 0);
  }
}

void setBackgroundColor(float r, float g, float b) {
  if (auto api = App::GetAPI(); api) {
    api->setBackgroundColor(r, g, b);
  }
}

// ----------------------------------------------------------------------------