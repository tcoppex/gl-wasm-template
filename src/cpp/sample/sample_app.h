#ifndef SAMPLE_APP_H_
#define SAMPLE_APP_H_

#include "framework/base_app.h"

// ----------------------------------------------------------------------------

/* Expose methods that will be accessible to the API. */
struct SampleAPI {
  virtual ~SampleAPI() = default;
  virtual void setBackgroundColor(float r, float g, float b) = 0;
};

// ----------------------------------------------------------------------------

class SampleApp : public BaseApp
                , public SampleAPI {
 private:
  static SampleAPI *sSampleAPI;

 public:
  static SampleAPI& GetSampleAPI() { 
    return *sSampleAPI; 
  }

  SampleApp() {
    sSampleAPI = this;
  }

  virtual ~SampleApp();

  /* [API] Set the background color. */
  void setBackgroundColor(float r, float g, float b) final {
    clearColor_[0] = r;
    clearColor_[1] = g;
    clearColor_[2] = b;
  }

 protected:
  void setup() final;
  void update() final;
  void draw() final;

 private:
  void setupShader() noexcept;
  void setupTexture() noexcept;
  void setupGeometry() noexcept;

 private:
  struct {
    GLuint program = 0u;
    GLuint texture = 0u;
    GLuint vao = 0u;
    GLuint vbo = 0u;
  } gl_;

  float clearColor_[3]{0.85f, 0.75f, 0.5f};
};

// ----------------------------------------------------------------------------

#endif // SAMPLE_APP_H_