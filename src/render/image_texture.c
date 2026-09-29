#include "image_texture.h"
#include <stdio.h>

/* stb_image ist eine "single-header library":
 * Genau EINE C-Datei muss STB_IMAGE_IMPLEMENTATION definieren,
 * bevor sie den Header inkludiert. */
#define STB_IMAGE_IMPLEMENTATION
#include "../stb_image.h"

int image_texture_load(const char* path, ImageTexture* outTex)
{
    if (path == NULL || outTex == NULL)
        return 0;

    outTex->id = 0;
    outTex->width = 0;
    outTex->height = 0;

    /* Bilder vertikal flippen: OpenGL erwartet Origin unten links,
     * die meisten Bildformate speichern Origin oben links. */
    stbi_set_flip_vertically_on_load(1);

    int channels;
    unsigned char* data = stbi_load(path, &outTex->width, &outTex->height,
                                    &channels, 0);
    if (data == NULL)
    {
        fprintf(stderr, "Bild konnte nicht geladen werden: %s\n", path);
        return 0;
    }

    /* Format bestimmen anhand der Kanaele */
    GLenum format;
    if (channels == 1)      format = GL_RED;
    else if (channels == 3) format = GL_RGB;
    else if (channels == 4) format = GL_RGBA;
    else
    {
        fprintf(stderr, "Unbekanntes Bildformat (%d Kanaele): %s\n", channels, path);
        stbi_image_free(data);
        return 0;
    }

    /* OpenGL-Textur erstellen */
    glGenTextures(1, &outTex->id);
    if (outTex->id == 0)
    {
        stbi_image_free(data);
        return 0;
    }

    glBindTexture(GL_TEXTURE_2D, outTex->id);

    /* Wrap: REPEAT, damit die Textur sich auf grossen Flaechen wiederholt */
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    /* Filter: linear + mipmaps fuer schoene Darstellung */
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, format,
                 outTex->width, outTex->height, 0,
                 format, GL_UNSIGNED_BYTE, data);

    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(data);

    return 1;
}

void image_texture_bind(const ImageTexture* texture, GLenum textureUnit)
{
    if (texture == NULL || texture->id == 0)
        return;

    glActiveTexture(textureUnit);
    glBindTexture(GL_TEXTURE_2D, texture->id);
}

void image_texture_delete(ImageTexture* texture)
{
    if (texture == NULL)
        return;

    if (texture->id != 0)
    {
        glDeleteTextures(1, &texture->id);
    }

    texture->id = 0;
    texture->width = 0;
    texture->height = 0;
}