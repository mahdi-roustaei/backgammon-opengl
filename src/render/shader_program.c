#include "shader_program.h"

#include <stdio.h>
#include <stdlib.h>

static char *read_text_file(const char *path)
{
    FILE *file;
    long length;
    char *content;

    if (path == NULL)
    {
        return NULL;
    }

    file = fopen(path, "rb");
    if (file == NULL)
    {
        printf("Could not open shader file: %s\n", path);
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    length = ftell(file);
    rewind(file);

    if (length <= 0)
    {
        fclose(file);
        return NULL;
    }

    content = (char *)malloc((size_t)length + 1);

    if (content == NULL)
    {
        fclose(file);
        return NULL;
    }

    if (fread(content, 1, (size_t)length, file) != (size_t)length)
    {
        free(content);
        fclose(file);
        return NULL;
    }

    content[length] = '\0';
    fclose(file);

    return content;
}

static GLuint compile_shader(GLenum type, const char *source)
{
    GLuint shader;
    GLint success;
    char log[1024];

    shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);

    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(shader, sizeof(log), NULL, log);
        printf("Shader compile error:\n%s\n", log);
        glDeleteShader(shader);
        return 0;
    }

    return shader;
}

GLuint shader_create_program_from_files(const char *vertex_path, const char *fragment_path)
{
    char *vertex_source;
    char *fragment_source;
    GLuint vertex_shader;
    GLuint fragment_shader;
    GLuint program;
    GLint success;
    char log[1024];

    vertex_source = read_text_file(vertex_path);
    fragment_source = read_text_file(fragment_path);

    if (vertex_source == NULL || fragment_source == NULL)
    {
        free(vertex_source);
        free(fragment_source);
        return 0;
    }

    vertex_shader = compile_shader(GL_VERTEX_SHADER, vertex_source);
    fragment_shader = compile_shader(GL_FRAGMENT_SHADER, fragment_source);

    free(vertex_source);
    free(fragment_source);

    if (vertex_shader == 0 || fragment_shader == 0)
    {
        glDeleteShader(vertex_shader);
        glDeleteShader(fragment_shader);
        return 0;
    }

    program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    glGetProgramiv(program, GL_LINK_STATUS, &success);

    if (!success)
    {
        glGetProgramInfoLog(program, sizeof(log), NULL, log);
        printf("Shader link error:\n%s\n", log);
        glDeleteProgram(program);
        return 0;
    }

    return program;
}

void shader_delete_program(GLuint program)
{
    if (program != 0)
    {
        glDeleteProgram(program);
    }
}
