#include "sample/api/lib.h"

#include <iostream>
#include "sample/sample_app.h"

// ----------------------------------------------------------------------------

void setBackgroundColor(float r, float g, float b) {
  SampleApp::GetSampleAPI().setBackgroundColor(r, g, b);
}

// ----------------------------------------------------------------------------