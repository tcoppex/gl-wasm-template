#include "cpp/app.h"

#include <cstdlib>
#include <cassert>

#include <array>
#include <iostream>

#if defined(EMSCRIPTEN)
#include "emscripten/emscripten.h"
#include "emscripten/html5.h"
// #include "emscripten/bind.h" // [requires RTTI]
#endif

// ----------------------------------------------------------------------------

static constexpr std::array<GLfloat, 16> kVertices{
  -0.75f, +0.75f,   0.f, 1.f,
  -0.75f, -0.75f,   0.f, 0.f,
  +0.75f, +0.75f,   1.f, 1.f,
  +0.75f, -0.75f,   1.f, 0.f
};

static constexpr std::array<GLchar const*, 512> kVertexShader{R"glsl(#version 300 es
layout(location = 0) in vec4 aPosition;
layout(location = 1) in vec2 aTexCoord;

out vec2 vTexCoord;

void main() {
  vTexCoord = aTexCoord;
  gl_Position = vec4(aPosition.xyz, 1.0);
})glsl"};

static constexpr std::array<GLchar const*, 512> kFragmentShader{R"glsl(#version 300 es
precision mediump float;

uniform sampler2D uTexture;

in vec2 vTexCoord;

out vec4 fragColor;

void main() {
  fragColor = texture(uTexture, vTexCoord).rgba;
})glsl"};

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

/* Custom JS function. */
EM_JS(void, my_js_alert, (char const* msg), {
  const s = UTF8ToString(msg);
  alert(s);
  console.log('Console log : "' + s + '"');
});

void init_js_api() {
  // Call custom initializer from js to bind C++ code to it.
  EM_ASM( initialize_custom_cppjs_api(), /**/ );

  // Custom js function.
  // my_js_alert( "Custom cpp-js API initialized !" );
}

} // namespace ""

#endif // EMSCRIPTEN

// ----------------------------------------------------------------------------

// [poor's man singleton]
static SimpleAPI* sApplicationAPI = nullptr;

SimpleAPI* App::GetAPI() {
  return sApplicationAPI;
}

// ----------------------------------------------------------------------------

App::App() {
  if (!createWindow()) {
    return;
  }

# if defined(EMSCRIPTEN)
  sApplicationAPI = this;
  init_js_api();
# endif
}

App::~App() {
  if (nullptr == window_) {
    return;
  }
  glDeleteBuffers(1, &gl_.vbo);
  glDeleteVertexArrays(1, &gl_.vao);
  glDeleteProgram(gl_.texture);
  glDeleteProgram(gl_.program);
}

int App::run() noexcept {
  if (!window_) {
    return EXIT_FAILURE;
  }
  setup();

  // One frame of the mainloop.
  static App* HigherSelf = this;
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

void App::setup() {
  setupShader();
  setupTexture();
  setupGeometry();
}

void App::update() {
}

void App::draw() {
  glClearColor(clearColor_[0], clearColor_[1], clearColor_[2], 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glUseProgram(gl_.program);
  glBindTexture(GL_TEXTURE_2D, gl_.texture);

  glBindVertexArray(gl_.vao);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
  glBindVertexArray(0u);

  glBindTexture(GL_TEXTURE_2D, 0);
  glUseProgram(0u);
}

// ----------------------------------------------------------------------------

bool App::createWindow() noexcept {
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

void App::setupEvents() noexcept {
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

bool App::pollEvents() noexcept {
  glfwPollEvents();
  return !glfwWindowShouldClose(window_);
}

void App::swapBuffers() noexcept {
  glfwSwapBuffers(window_);
}

// ----------------------------------------------------------------------------

void App::setupShader() noexcept {
  gl_.program = glCreateProgram();
  const GLuint vshader = glCreateShader(GL_VERTEX_SHADER);
  const GLuint fshader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(vshader, 1, kVertexShader.data(), nullptr);
  glShaderSource(fshader, 1, kFragmentShader.data(), nullptr);
  glCompileShader(vshader);
  glCompileShader(fshader);
  glAttachShader(gl_.program, vshader);
  glAttachShader(gl_.program, fshader);
  glDeleteShader(vshader);
  glDeleteShader(fshader);
  glLinkProgram(gl_.program);
  // CHECK_GL_ERRORS();

  GLint program_linked;
  glGetProgramiv(gl_.program, GL_LINK_STATUS, &program_linked);
  if (program_linked != GL_TRUE) {
    GLsizei log_length = 0;
    GLchar message[1024];
    glGetProgramInfoLog(gl_.program, 1024, &log_length, message);
    std::cerr << message << std::endl; 
  }
}

void App::setupTexture() noexcept {

}

void App::setupGeometry() noexcept {
  assert(0u != gl_.program);
  
  // Vertex Array.
  glGenVertexArrays(1, &gl_.vao);
  glGenBuffers(1, &gl_.vbo);

  // Vertex buffer.
  glBindVertexArray(gl_.vao);
  glBindBuffer(GL_ARRAY_BUFFER, gl_.vbo);
  const GLsizei bytesize = kVertices.size() * sizeof(kVertices[0]);
  glBufferData(GL_ARRAY_BUFFER, bytesize, kVertices.data(), GL_STATIC_DRAW);

  // Bind mesh attributes.
  glUseProgram(gl_.program);
  {
    const GLsizei stride = 4 * sizeof(float);

    // (position)
    GLint pos_attrib = glGetAttribLocation(gl_.program, "aPosition");
    glEnableVertexAttribArray(pos_attrib);
    glVertexAttribPointer(pos_attrib, 2, GL_FLOAT, GL_FALSE, stride, nullptr);

    // (texcoord)
    GLint uv_attrib = glGetAttribLocation(gl_.program, "aTexCoord");
    glEnableVertexAttribArray(uv_attrib);
    glVertexAttribPointer(uv_attrib, 2, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<GLvoid*>(2*sizeof(float)));

    // Set texture uniform.
    const int texture_unit = 0;
    const GLint loc = glGetUniformLocation(gl_.program, "uFrameTexture");
    glActiveTexture(GL_TEXTURE0 + texture_unit);
    glUniform1i(loc, texture_unit);
  }
  glUseProgram(0u);
  glBindVertexArray(0u);
  
  // CHECK_GL_ERRORS();  
}

// ----------------------------------------------------------------------------
