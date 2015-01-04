#ifndef WINDOW_H
#define WINDOW_H
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <GL/glu.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "helpers.h"

typedef struct s_WindowWithContext {
  SDL_Window* window;
  SDL_GLContext context;
  bool is_closed;
} WindowWithContext;

WindowWithContext* create_window(int width, int height, const char* title);

void update_window(WindowWithContext* window, void (*render)(WindowWithContext*));

bool is_window_closed(WindowWithContext* window);

void destroy_window(WindowWithContext* window);
#endif
