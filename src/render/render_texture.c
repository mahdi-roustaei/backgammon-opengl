#include "render_texture.h"

int render_texture_create_from_ppm(RenderTexture *texture, const PpmImage *image)
{
    if (texture == NULL || image == NULL || image->pixels == NULL || image->width <= 0 || image->height <= 0)
    {
        return 0;
    }

    texture->id = 0;
    texture->width = image->width;
    texture->height = image->height;

    glGenTextures(1, &texture->id);

    if (texture->id == 0)
    {
        return 0;
    }

    glBindTexture(GL_TEXTURE_2D, texture->id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGB,
        image->width,
        image->height,
        0,
        GL_RGB,
        GL_UNSIGNED_BYTE,
        image->pixels
    );

    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);

    return 1;
}

void render_texture_bind(const RenderTexture *texture, GLenum texture_unit)
{
    if (texture == NULL || texture->id == 0)
    {
        return;
    }

    glActiveTexture(texture_unit);
    glBindTexture(GL_TEXTURE_2D, texture->id);
}

void render_texture_delete(RenderTexture *texture)
{
    if (texture == NULL)
    {
        return;
    }

    if (texture->id != 0)
    {
        glDeleteTextures(1, &texture->id);
    }

    texture->id = 0;
    texture->width = 0;
    texture->height = 0;
}
