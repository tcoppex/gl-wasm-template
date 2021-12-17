#ifndef APP_H_
#define APP_H_

#define GLFW_INCLUDE_ES3
#include "GLFW/glfw3.h"

// ----------------------------------------------------------------------------

class SimpleAPI {
  public:
    virtual ~SimpleAPI() = default;

    virtual void setBackgroundColor(float r, float g, float b) = 0;
};

// ----------------------------------------------------------------------------

class App : public SimpleAPI {
 public:
  static constexpr int kScreenWidth = 800;
  static constexpr int kScreenHeight = 600;

  /* Return a pointer to the API accessible methods. */
  static SimpleAPI* GetAPI();

 public:
  App();
  virtual ~App();

  /* Run the application mainloop. */
  int run() noexcept;

  /* [API] Set the background color. */
  void setBackgroundColor(float r, float g, float b) override {
    clearColor_[0] = r;
    clearColor_[1] = g;
    clearColor_[2] = b;
  }

 protected:
  virtual void setup();
  virtual void update();
  virtual void draw();

 private:
  bool createWindow() noexcept;
  void setupEvents() noexcept;
  bool pollEvents() noexcept;
  void swapBuffers() noexcept;

  void setupShader() noexcept;
  void setupTexture() noexcept;
  void setupGeometry() noexcept;

 private:
  GLFWwindow *window_ = nullptr;

  struct {
    GLuint program = 0u;
    GLuint texture = 0u;
    GLuint vao = 0u;
    GLuint vbo = 0u;
  } gl_;

  float clearColor_[3]{0.85f, 0.75f, 0.5f};
};

// ----------------------------------------------------------------------------

#endif // APP_H_