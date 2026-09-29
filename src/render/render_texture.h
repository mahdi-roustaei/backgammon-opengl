#ifndef RENDER_TEXTURE_H
#define RENDER_TEXTURE_H

#include <GL/glew.h>

#include "../texture/ppm_loader.h"

typedef struct
{
    GLuint id;
    int width;
    int height;
} RenderTexture;

int render_texture_create_from_ppm(RenderTexture *texture, const PpmImage *image);
void render_texture_bind(const RenderTexture *texture, GLenum texture_unit);
void render_texture_delete(RenderTexture *texture);

#endif
