#ifndef LIB_H_
#define LIB_H_

#include "emscripten.h"

// ----------------------------------------------------------------------------

/* Exported functions to be called inside javascript. */

extern "C" {

EMSCRIPTEN_KEEPALIVE void testCall();
EMSCRIPTEN_KEEPALIVE void printNumber(int f);
EMSCRIPTEN_KEEPALIVE int square(int c);

EMSCRIPTEN_KEEPALIVE void setVerticalSync(bool status);

EMSCRIPTEN_KEEPALIVE void setBackgroundColor(float r, float g, float b);

}

// ----------------------------------------------------------------------------

#endif // LIB_H_