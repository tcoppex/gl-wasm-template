#ifndef BASE_APP_H_
#define BASE_APP_H_

#define GLFW_INCLUDE_ES3
#include "GLFW/glfw3.h"

// ----------------------------------------------------------------------------

/* Expose methods that will be accessible to the API. */
struct BaseAPI {
  virtual ~BaseAPI() = default;
  virtual int getWidth() const = 0;
  virtual int getHeight() const = 0;
};

// ----------------------------------------------------------------------------

class BaseApp : public BaseAPI {
 private:
  static constexpr int kScreenWidth = 800;
  static constexpr int kScreenHeight = 600;

  static BaseAPI *sBaseAPI;

 public:
  static BaseAPI& GetBaseAPI() { 
    return *sBaseAPI; 
  }

  BaseApp();
  virtual ~BaseApp() = default;

  /* Run the application's mainloop. */
  int run() noexcept;

  /* Framework Public API accessible methods. */
  int getWidth() const override { return kScreenWidth; }
  int getHeight() const override { return kScreenHeight; }

 protected:
  virtual void setup() {}
  virtual void update() {}
  virtual void draw() {}

 private:
  bool createWindow() noexcept;
  void setupEvents() noexcept;
  bool pollEvents() noexcept;
  void swapBuffers() noexcept;

 private:
  GLFWwindow *window_ = nullptr;
};

// ----------------------------------------------------------------------------

#endif // BASE_APP_H_