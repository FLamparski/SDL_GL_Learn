#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <GL/glu.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "helpers.h"
#include "glprogram.h"
#include "window.h"

bool init_sdl() {
  return SDL_Init(SDL_INIT_EVERYTHING) < 0;
}

static GLProgram gl_program;

bool init_opengl() {
  gl_program.prog_id = glCreateProgram();
  pdebug("Creating program with id %d\n", gl_program.prog_id);

  GLuint vertex_shader = shader_from_file("shaders/four.vert", GL_VERTEX_SHADER);

  glCompileShader(vertex_shader);
  GLint is_vertex_shader_compiled = GL_FALSE;
  glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &is_vertex_shader_compiled);
  if (is_vertex_shader_compiled != GL_TRUE) {
    perr("Could not compile the vertex shader %d.\n", vertex_shader);
    print_shader_log(vertex_shader);
    return false;
  } else {
    pdebug("Vertex shader %d compiled.\n", vertex_shader);
  }

  glAttachShader(gl_program.prog_id, vertex_shader);

  GLuint fragment_shader = shader_from_file("shaders/four.frag", GL_FRAGMENT_SHADER);

  glCompileShader(fragment_shader);
  GLint is_fragment_shader_compiled = GL_FALSE;
  glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &is_fragment_shader_compiled);
  if (is_fragment_shader_compiled != GL_TRUE) {
    perr("Could not compile the fragment shader %d.\n", fragment_shader);
    print_shader_log(fragment_shader);
    return false;
  } else {
    pdebug("Fragment shader %d compiled.\n", fragment_shader);
  }

  glAttachShader(gl_program.prog_id, fragment_shader);

  pdebug("Linking program %d\n", gl_program.prog_id);
  glLinkProgram(gl_program.prog_id);

  GLint program_linked = GL_FALSE;
  glGetProgramiv(gl_program.prog_id, GL_LINK_STATUS, &program_linked);
  if (program_linked != GL_TRUE) {
    perr("Error linking program %d.\n", gl_program.prog_id);
    print_program_log(gl_program.prog_id);
    return false;
  }

  pdebug("Program %d is now fully linked.\n", gl_program.prog_id);
  return true;
}

bool setup_scene() {
  gl_program.vertex_pos2d_location = glGetAttribLocation(gl_program.prog_id, "LVertexPos2D");
  if (gl_program.vertex_pos2d_location == -1) {
    perr("Could not find LVertexPos2D GLSL variable.\n");
    return false;
  }

  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

  // VBO data
  GLfloat vbo_data[] = {
    -0.5f, -0.5f,
     0.5f, -0.5f,
     0.5f,  0.5f,
    -0.5f,  0.5f
  };

  // IBO data
  GLuint index_data[] = { 0, 1, 2, 3 };

  // Create the VBO
  glGenBuffers(1, &gl_program.vbo);
  glBindBuffer(GL_ARRAY_BUFFER, gl_program.vbo);
  glBufferData(GL_ARRAY_BUFFER, 2 * 4 * sizeof(GLfloat), vbo_data, GL_STATIC_DRAW);

  // Create the IBO
  glGenBuffers(1, &gl_program.ibo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gl_program.ibo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4 * sizeof(GLuint), index_data, GL_STATIC_DRAW);

  return true;
}

void render (WindowWithContext* window) {
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  glUseProgram(gl_program.prog_id);

  // make sure to maintain the window's aspect ratio
  int sdl_window_width, sdl_window_height;
  SDL_GetWindowSize(window->window, &sdl_window_width, &sdl_window_height);
  GLuint shader_window_width = glGetUniformLocation(gl_program.prog_id, "window_width");
  glUniform1i(shader_window_width, sdl_window_width);
  GLuint shader_window_height = glGetUniformLocation(gl_program.prog_id, "window_height");
  glUniform1i(shader_window_height, sdl_window_height);

  glBindBuffer(GL_ARRAY_BUFFER, gl_program.vbo);
  glEnableVertexAttribArray(gl_program.vertex_pos2d_location);
  glVertexAttribPointer(gl_program.vertex_pos2d_location, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), NULL);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gl_program.ibo);
  glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_INT, NULL);

  SDL_GL_SwapWindow(window->window);

  glDisableVertexAttribArray(gl_program.vertex_pos2d_location);
  glUseProgram(0);
}

void print_diagnostics() {
  printf(
      "OpenGL version: %s\n"
      "Vendor: %s\n"
      "Renderer: %s\n"
      "Shading language version(s): %s\n",
      glGetString(GL_VERSION),
      glGetString(GL_VENDOR),
      glGetString(GL_RENDERER),
      glGetString(GL_SHADING_LANGUAGE_VERSION)
  );
}
int main() {
  int returncode = 0;
  WindowWithContext* window = create_window(800, 600, "SDL+OpenGL Window");
  if (!window) {
    SDL_Quit();
    return 1;
  }
  pinfo("Created an OpenGL window at %p.\n", window);
#ifndef NDEBUG
  printf("-------------------------------------\n");
  print_diagnostics();
  printf("-------------------------------------\n");
#endif

  bool ok = init_opengl();
  pinfo("Shader setup: %s\n", ok ? "ok" : "not ok");
  if (!ok) goto exit_main;

  ok = setup_scene();
  pinfo("Scene setup: %s\n", ok ? "ok" : "not ok");
  if (!ok) goto exit_main;

  while (!is_window_closed(window)) {
    update_window(window, render);
  }

exit_main:
  pinfo("Tearing down the window + context at %p.\n", window);
  destroy_window(window);
  if (gl_program.prog_id) glDeleteProgram(gl_program.prog_id);
  SDL_Quit();
  pdebug("Terminating.\n");
  return returncode;
}
