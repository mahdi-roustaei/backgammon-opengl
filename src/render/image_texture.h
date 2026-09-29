#ifndef IMAGE_TEXTURE_H
#define IMAGE_TEXTURE_H

#include <GL/glew.h>

/*
 * RenderTexture-aequivalent fuer JPG/PNG via stb_image.
 * Hat die gleiche Struktur wie Sepehrs RenderTexture, damit wir
 * sie zusammen verwenden koennen.
 */
typedef struct
{
    GLuint id;
    int width;
    int height;
} ImageTexture;

/*
 * Laedt ein Bild (JPG/PNG) und erzeugt eine OpenGL-Textur.
 * Rueckgabe: 1 = Erfolg, 0 = Fehler (Sepehrs Konvention).
 */
int image_texture_load(const char* path, ImageTexture* outTex);

/*
 * Bindet die Textur an die angegebene Textur-Einheit (z.B. GL_TEXTURE0).
 */
void image_texture_bind(const ImageTexture* texture, GLenum textureUnit);

/*
 * Loescht die Textur und gibt den GPU-Speicher frei.
 */
void image_texture_delete(ImageTexture* texture);

#endif