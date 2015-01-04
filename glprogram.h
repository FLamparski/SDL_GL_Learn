#ifndef GL_PROGRAM_H
#define GL_PROGRAM_H
#include <GL/glew.h>

typedef struct _gl_prog {
  GLuint prog_id;
  GLint vertex_pos2d_location;
  GLuint vbo;
  GLuint ibo;
} GLProgram;
#endif
