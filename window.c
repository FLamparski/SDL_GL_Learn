#include "window.h"
#include "glprogram.h"
#include "configure.h"

WindowWithContext* create_window(int width, int height, const char* title) {
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, CONFIG_OPENGL_MAJOR);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, CONFIG_OPENGL_MINOR);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

  pdebug("Creating a %dx%d window\n", width, height);
  SDL_Window* window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
  if (!window) {
    perr("Could not create a window. SDL error: %s.\n", SDL_GetError());
    return NULL;
  }

  SDL_GLContext context = SDL_GL_CreateContext(window);
  if (!context) {
    SDL_DestroyWindow(window);
    perr("Could not create an OpenGL context. SDL error: %s.\n", SDL_GetError());
    return NULL;
  } else {
    glewExperimental = GL_TRUE;
    GLenum glewError = glewInit();
    if (glewError != GLEW_OK) {
      perr("Could not load GLEW: %s\n.", glewGetErrorString(glewError));
    }
  }

  SDL_GL_SetSwapInterval(1);

  WindowWithContext win_local = { .window = window, .context = context };
  WindowWithContext* win = (WindowWithContext*) malloc(sizeof(WindowWithContext));
  *win = win_local;

  return win;
}

void update_window(WindowWithContext* window, void (*render)(WindowWithContext*)) {
  SDL_Event ev;

  SDL_PollEvent(&ev);

  if (ev.type == SDL_QUIT) {
    window->is_closed = true;
  }

  render(window);
}

bool is_window_closed(WindowWithContext* window) {
  return window->is_closed;
}

void destroy_window(WindowWithContext* window) {
  SDL_GL_DeleteContext(window->context);
  SDL_DestroyWindow(window->window);
  free(window);
}
