#include "framework/base_app.h"

#include <cstdlib>
#include <cassert>

#include <array>
#include <iostream>

#if defined(EMSCRIPTEN)
#include <emscripten/emscripten.h>
#include <emscripten/html5.h>
#endif

// ----------------------------------------------------------------------------

BaseAPI *BaseApp::sBaseAPI = nullptr;

// ----------------------------------------------------------------------------

#if defined(EMSCRIPTEN)

namespace {

struct WebData_t {
  int width;
  int height;
};

static WebData_t sWEB;

EM_BOOL onKeyPress(int eventType, const EmscriptenKeyboardEvent *keyEvent, void *userData) {
  std::cerr << __FUNCTION__ << std::endl;
  return EM_TRUE;
}

EM_BOOL onKeyDown(int eventType, const EmscriptenKeyboardEvent *keyEvent, void *userData) {
  std::cerr << __FUNCTION__ << std::endl;
  return EM_TRUE;
}

EM_BOOL onKeyUp(int eventType, const EmscriptenKeyboardEvent *keyEvent, void *userData) {
  std::cerr << __FUNCTION__ << std::endl;
  return EM_TRUE;
}

EM_BOOL onMouseMove(int eventType, const EmscriptenMouseEvent *mouseEvent, void *userData) {
  std::cerr << __FUNCTION__ << std::endl;
  return EM_TRUE;
}

EM_BOOL onMouseUp(int eventType, const EmscriptenMouseEvent *mouseEvent, void *userData) {
  std::cerr << __FUNCTION__ << std::endl;
  return EM_TRUE;
}

EM_BOOL onMouseDown(int eventType, const EmscriptenMouseEvent *mouseEvent, void *userData) {
  std::cerr << __FUNCTION__ << std::endl;
  return EM_TRUE;
}

EM_BOOL onMouseEnter(int eventType, const EmscriptenMouseEvent *mouseEvent, void *userData) {
  std::cerr << __FUNCTION__ << std::endl;
  return EM_TRUE;
}

EM_BOOL onMouseLeave(int eventType, const EmscriptenMouseEvent *mouseEvent, void *userData) {
  std::cerr << __FUNCTION__ << std::endl;
  return EM_TRUE;
}

EM_BOOL onTouchStart(int eventType, const EmscriptenTouchEvent *touchEvent, void *userData) {
  std::cerr << __FUNCTION__ << std::endl;
  return EM_TRUE;
}

EM_BOOL onTouchEnd(int eventType, const EmscriptenTouchEvent *touchEvent, void *userData) {
  std::cerr << __FUNCTION__ << std::endl;
  return EM_TRUE;
}

EM_BOOL onTouchMove(int eventType, const EmscriptenTouchEvent *touchEvent, void *userData) {
  std::cerr << __FUNCTION__ << std::endl;
  return EM_TRUE;
}

EM_BOOL onTouchCancel(int eventType, const EmscriptenTouchEvent *touchEvent, void *userData) {
  std::cerr << __FUNCTION__ << std::endl;
  return EM_TRUE;
}

EM_BOOL onResize(int eventType, const EmscriptenUiEvent *uiEvent, void *userData) {
  std::cerr << __FUNCTION__ << std::endl;
  return EM_TRUE;
}

} // namespace ""

#endif // EMSCRIPTEN

// ----------------------------------------------------------------------------

BaseApp::BaseApp() {
  if (!createWindow()) {
    return;
  }
  sBaseAPI = this;
}

int BaseApp::run() noexcept {
# if defined(EMSCRIPTEN)
  // Call custom initializer from js to bind C++ code to it.
  EM_ASM( initialize_cpp_js_api(), /**/ );
# endif

  if (!window_) {
    return EXIT_FAILURE;
  }
  setup();

  // One frame of the mainloop.
  static BaseApp* HigherSelf = this;
  auto frame = []() {
    HigherSelf->update();
    HigherSelf->draw();
    HigherSelf->swapBuffers();
  };

# if defined(EMSCRIPTEN)
  // Simulate infinite loop with browser controlled framerate.
  emscripten_set_main_loop(frame, -1, true);
# else
  while (pollEvents()) {
    frame();
  }
# endif

  return EXIT_SUCCESS;
}

// ----------------------------------------------------------------------------

bool BaseApp::createWindow() noexcept {
  glfwSetErrorCallback([](int error, const char *msg) {
    std::cerr << "GLFW Error [" << std::to_string(error)  << "] : "  << msg << std::endl 
              << std::endl;
    exit(EXIT_FAILURE);
  });
  
  if (!glfwInit()) {
    std::cerr << "GLFW failed to be initialized." << std::endl;
    return false;
  }
  atexit(glfwTerminate);

  glfwDefaultWindowHints();
  glfwWindowHint( GLFW_OPENGL_PROFILE,            GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint( GLFW_CLIENT_API,                GLFW_OPENGL_API);
  glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT,     GLFW_TRUE);
  glfwWindowHint( GLFW_OPENGL_DEBUG_CONTEXT,      GL_TRUE);
  glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR,     3);
  glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR,     3);

  // Render context.
  glfwWindowHint( GLFW_SAMPLES,                   GLFW_DONT_CARE);
  glfwWindowHint( GLFW_DOUBLEBUFFER,              GLFW_TRUE);
  glfwWindowHint( GLFW_VISIBLE,                   GLFW_TRUE);
  glfwWindowHint( GLFW_RESIZABLE,                 GLFW_TRUE);
  glfwWindowHint( GLFW_MAXIMIZED,                 GLFW_FALSE);
  
  window_ = glfwCreateWindow( kScreenWidth, kScreenHeight, "GFLW Window", nullptr, nullptr);
  if (!window_) {
    std::cerr << "The render context creation failed." << std::endl;
    return false;
  }
  glfwMakeContextCurrent(window_);
  setupEvents();

  // Note: On emscripten setting the swapInterval before the mainloop create a warning.
  glfwSwapInterval(1);
  
  return true;
}

void BaseApp::setupEvents() noexcept {
# if defined(EMSCRIPTEN)
  {
    EMSCRIPTEN_RESULT res = EMSCRIPTEN_RESULT_SUCCESS;
    
    constexpr void* userData = nullptr;
    constexpr bool useCapture = true;
    constexpr char const* id = "#canvas"; // [todo : provide in the constructor]
    
    // Keys callbacks.
    res |= emscripten_set_keypress_callback(id, userData, !useCapture, onKeyPress);
    res |= emscripten_set_keydown_callback(id, userData, !useCapture, onKeyDown);
    res |= emscripten_set_keyup_callback(id, userData, !useCapture, onKeyUp);

    // Mouse callbacks.
    res |= emscripten_set_mousemove_callback(id, userData, !useCapture, onMouseMove);
    res |= emscripten_set_mouseup_callback(id, userData, useCapture, onMouseUp);
    res |= emscripten_set_mousedown_callback(id, userData, useCapture, onMouseDown);
    // res |= emscripten_set_click_callback(id, userData, useCapture, em_mouse_callback_func callback);
    // res |= emscripten_set_dblclick_callback(id, userData, useCapture, em_mouse_callback_func callback);
    res |= emscripten_set_mouseenter_callback(id, userData, !useCapture, onMouseEnter);
    res |= emscripten_set_mouseleave_callback(id, userData, !useCapture, onMouseLeave);

    // Touch callbacks.
    res |= emscripten_set_touchstart_callback(id, userData, useCapture, onTouchStart);
    res |= emscripten_set_touchmove_callback(id, userData, useCapture, onTouchMove);
    res |= emscripten_set_touchend_callback(id, userData, useCapture, onTouchEnd);
    res |= emscripten_set_touchcancel_callback(id, userData, useCapture, onTouchCancel); 

    // Surface size callbacks.
    res |= emscripten_set_resize_callback(id, userData, !useCapture, onResize);

    if (res != EMSCRIPTEN_RESULT_SUCCESS) {
      std::cerr << "Some callbacks initialization failed." << std::endl;
    }

    double w, h;
    emscripten_get_element_css_size(id, &w, &h);

    sWEB.width  = static_cast<int>(w);
    sWEB.height = static_cast<int>(h);
    emscripten_set_canvas_element_size(id, sWEB.width, sWEB.height);
  }
# endif

  glfwSetWindowSizeCallback(window_, [](GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
  });

  glfwSetKeyCallback(window_, [](GLFWwindow *window, int key, int scancode, int action, int mods) {  
    if ((action == GLFW_PRESS) && (key == GLFW_KEY_ESCAPE)) {
      glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
  });
}

bool BaseApp::pollEvents() noexcept {
  glfwPollEvents();
  return !glfwWindowShouldClose(window_);
}

void BaseApp::swapBuffers() noexcept {
  glfwSwapBuffers(window_);
}

// ----------------------------------------------------------------------------
