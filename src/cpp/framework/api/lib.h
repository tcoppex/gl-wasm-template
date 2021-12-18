#ifndef FRAMEWORK_API_LIB_H_
#define FRAMEWORK_API_LIB_H_

#include <emscripten/em_macros.h>
// #define EMSCRIPTEN_KEEPALIVE __attribute__((used))

// ----------------------------------------------------------------------------

/* Exported functions to be called inside javascript. */

extern "C" {

EMSCRIPTEN_KEEPALIVE void setVerticalSync(bool status);

EMSCRIPTEN_KEEPALIVE int getSurfaceWidth();

EMSCRIPTEN_KEEPALIVE int getSurfaceHeight();

}

// ----------------------------------------------------------------------------

#endif // FRAMEWORK_API_LIB_H_