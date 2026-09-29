#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

#include <GL/glew.h>

GLuint shader_create_program_from_files(const char *vertex_path, const char *fragment_path);
void shader_delete_program(GLuint program);

#endif
