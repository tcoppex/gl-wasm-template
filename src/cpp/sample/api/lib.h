#ifndef SAMPLE_API_LIB_H_
#define SAMPLE_API_LIB_H_

#include "framework/api/lib.h"

// ----------------------------------------------------------------------------

extern "C" {

EMSCRIPTEN_KEEPALIVE void setBackgroundColor(float r, float g, float b);

}

// ----------------------------------------------------------------------------

#endif // SAMPLE_API_LIB_H_