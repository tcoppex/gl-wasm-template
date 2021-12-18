#include "sample/sample_app.h"

#include <cassert>
#include <array>
#include <iostream>

// ----------------------------------------------------------------------------

SampleAPI *SampleApp::sSampleAPI = nullptr;

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

SampleApp::~SampleApp() {
  if (!gl_.vao) {
    return;
  }
  
  glDeleteBuffers(1, &gl_.vbo);
  glDeleteVertexArrays(1, &gl_.vao);
  glDeleteProgram(gl_.texture);
  glDeleteProgram(gl_.program);
}

// ----------------------------------------------------------------------------

void SampleApp::setup() {
  setupShader();
  setupTexture();
  setupGeometry();
}

void SampleApp::update() {
}

void SampleApp::draw() {
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

void SampleApp::setupShader() noexcept {
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

void SampleApp::setupTexture() noexcept {

}

void SampleApp::setupGeometry() noexcept {
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
