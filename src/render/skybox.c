#include "skybox.h"
#include "../stb_image.h"
#include <stdio.h>
#include <stddef.h>

/* Wuerfel-Geometrie fuer Skybox (Position-Daten, 36 Vertices, 12 Triangles).
 * Wuerfel von -1 bis +1. Die Skybox-Shader interpretieren diese Positionen
 * direkt als Sample-Richtung fuer die Cubemap. */
static const float skyboxVertices[] = {
    /* +Z (vorne) */
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

    /* -Z (hinten) */
    -1.0f,  1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,

    /* -X (links) */
    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,

    /* +X (rechts) */
     1.0f, -1.0f,  1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f, -1.0f,  1.0f,

    /* +Y (oben) */
    -1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,

    /* -Y (unten) */
    -1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f,  1.0f,
     1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f
};

/* Laedt EIN Cubemap-Face aus einer JPG/PNG-Datei.
 * face: GL_TEXTURE_CUBE_MAP_POSITIVE_X usw.
 * Rueckgabe: 1 = Erfolg, 0 = Fehler. */
static int loadFace(GLenum face, const char* path)
{
    int width, height, channels;

    /* Skybox-Bilder duerfen NICHT geflippt werden (anders als normale Texturen) */
    stbi_set_flip_vertically_on_load(0);

    unsigned char* data = stbi_load(path, &width, &height, &channels, 0);
    if (data == NULL)
    {
        fprintf(stderr, "Skybox-Bild konnte nicht geladen werden: %s\n", path);
        return 0;
    }

    GLenum format;
    if (channels == 3) format = GL_RGB;
    else if (channels == 4) format = GL_RGBA;
    else
    {
        fprintf(stderr, "Skybox: Unbekanntes Bildformat (%d Kanaele): %s\n", channels, path);
        stbi_image_free(data);
        return 0;
    }

    glTexImage2D(face, 0, format, width, height, 0,
                 format, GL_UNSIGNED_BYTE, data);

    stbi_image_free(data);
    return 1;
}

int skybox_load(Skybox* outSkybox,
                const char* posX, const char* negX,
                const char* posY, const char* negY,
                const char* posZ, const char* negZ)
{
    if (outSkybox == NULL) return 0;

    outSkybox->cubemapId = 0;
    outSkybox->vao = 0;
    outSkybox->vbo = 0;

    /* === Cubemap-Textur erstellen === */
    glGenTextures(1, &outSkybox->cubemapId);
    glBindTexture(GL_TEXTURE_CUBE_MAP, outSkybox->cubemapId);

    /* Sechs Faces laden. Bei Fehler: Textur freigeben statt zu lecken. */
    if (!loadFace(GL_TEXTURE_CUBE_MAP_POSITIVE_X, posX) ||
        !loadFace(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, negX) ||
        !loadFace(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, posY) ||
        !loadFace(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, negY) ||
        !loadFace(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, posZ) ||
        !loadFace(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, negZ))
    {
        glDeleteTextures(1, &outSkybox->cubemapId);
        outSkybox->cubemapId = 0;
        return 0;
    }

    /* Filter und Wrap-Mode */
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    /* === Wuerfel-Geometrie hochladen === */
    glGenVertexArrays(1, &outSkybox->vao);
    glGenBuffers(1, &outSkybox->vbo);

    glBindVertexArray(outSkybox->vao);
    glBindBuffer(GL_ARRAY_BUFFER, outSkybox->vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);

    /* Attribut 0: Position (3 floats) */
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    return 1;
}

void skybox_draw(const Skybox* skybox, GLuint shaderProgram)
{
    if (skybox == NULL || skybox->cubemapId == 0) return;

    /* Tiefen-Test so anpassen, dass die Skybox immer im Hintergrund liegt */
    glDepthFunc(GL_LEQUAL);

    glUseProgram(shaderProgram);

    /* Cubemap an Textur-Einheit 0 binden */
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skybox->cubemapId);
    GLint loc = glGetUniformLocation(shaderProgram, "skyboxCube");
    glUniform1i(loc, 0);

    glBindVertexArray(skybox->vao);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);

    /* Tiefen-Funktion zuruecksetzen */
    glDepthFunc(GL_LESS);
}

void skybox_delete(Skybox* skybox)
{
    if (skybox == NULL) return;

    if (skybox->cubemapId != 0) glDeleteTextures(1, &skybox->cubemapId);
    if (skybox->vbo != 0) glDeleteBuffers(1, &skybox->vbo);
    if (skybox->vao != 0) glDeleteVertexArrays(1, &skybox->vao);

    skybox->cubemapId = 0;
    skybox->vbo = 0;
    skybox->vao = 0;
}