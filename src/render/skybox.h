#ifndef SKYBOX_H
#define SKYBOX_H

#include <GL/glew.h>

/*
 * Skybox-Struktur:
 * - cubemapId: OpenGL-Textur-ID einer Cubemap (sechs Bilder als Wuerfel)
 * - vao/vbo:   Wuerfel-Geometrie zum Rendern der Skybox
 */
typedef struct
{
    GLuint cubemapId;
    GLuint vao;
    GLuint vbo;
} Skybox;

/*
 * Laedt die Skybox aus sechs JPG/PNG-Dateien.
 * Reihenfolge: +X, -X, +Y, -Y, +Z, -Z (OpenGL-Cubemap-Konvention).
 * Erzeugt zusaetzlich eine Wuerfel-Geometrie (VAO/VBO).
 * Rueckgabe: 1 = Erfolg, 0 = Fehler.
 */
int skybox_load(Skybox* outSkybox,
                const char* posX, const char* negX,
                const char* posY, const char* negY,
                const char* posZ, const char* negZ);

/*
 * Rendert die Skybox.
 * Muss VOR allen anderen Objekten aufgerufen werden.
 * shaderProgram: vorher mit glUseProgram() aktiviertes Skybox-Shader-Programm.
 */
void skybox_draw(const Skybox* skybox, GLuint shaderProgram);

/*
 * Loescht die Skybox aus dem GPU-Speicher.
 */
void skybox_delete(Skybox* skybox);

#endif