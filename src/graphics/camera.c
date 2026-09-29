#include "camera.h"
#include "../math/mat4.h"
#include <math.h>
#include <assert.h>
#include <stddef.h>


#define PI 3.14159265358979f
#define DEG_TO_RAD(deg) ((deg) * PI / 180.0f)

/*
 * Berechnet front, right und up neu, basierend auf yaw und pitch.
 * Wird nach jeder Aenderung der Blickrichtung aufgerufen.
 */
static void updateVectors(Camera* camera)
{
    float yawRad = DEG_TO_RAD(camera->yaw);
    float pitchRad = DEG_TO_RAD(camera->pitch);

    /* front-Vektor aus den Winkeln berechnen (Spherical Coordinates) */
    camera->front[0] = cosf(yawRad) * cosf(pitchRad);
    camera->front[1] = sinf(pitchRad);
    camera->front[2] = sinf(yawRad) * cosf(pitchRad);

    /* Normieren */
    float len = sqrtf(camera->front[0]*camera->front[0] +
                      camera->front[1]*camera->front[1] +
                      camera->front[2]*camera->front[2]);
    camera->front[0] /= len;
    camera->front[1] /= len;
    camera->front[2] /= len;

    /* right = front x worldUp, normiert */
    camera->right[0] = camera->front[1]*camera->worldUp[2] - camera->front[2]*camera->worldUp[1];
    camera->right[1] = camera->front[2]*camera->worldUp[0] - camera->front[0]*camera->worldUp[2];
    camera->right[2] = camera->front[0]*camera->worldUp[1] - camera->front[1]*camera->worldUp[0];
    len = sqrtf(camera->right[0]*camera->right[0] +
                camera->right[1]*camera->right[1] +
                camera->right[2]*camera->right[2]);
    camera->right[0] /= len;
    camera->right[1] /= len;
    camera->right[2] /= len;

    /* up = right x front, normiert */
    camera->up[0] = camera->right[1]*camera->front[2] - camera->right[2]*camera->front[1];
    camera->up[1] = camera->right[2]*camera->front[0] - camera->right[0]*camera->front[2];
    camera->up[2] = camera->right[0]*camera->front[1] - camera->right[1]*camera->front[0];
}

void cameraInit(Camera* camera, float posX, float posY, float posZ)
{
    assert(camera != NULL);

    camera->position[0] = posX;
    camera->position[1] = posY;
    camera->position[2] = posZ;

    camera->worldUp[0] = 0.0f;
    camera->worldUp[1] = 1.0f;
    camera->worldUp[2] = 0.0f;

    /* yaw = -90 bedeutet, dass die Kamera zu Beginn entlang -Z schaut */
    camera->yaw = -90.0f;
    camera->pitch = 0.0f;

    camera->movementSpeed = 2.5f;
    camera->mouseSensitivity = 0.1f;

    updateVectors(camera);
}

void cameraGetViewMatrix(const Camera* camera, float* out)
{
    /* target = position + front */
    float target[3] = {
        camera->position[0] + camera->front[0],
        camera->position[1] + camera->front[1],
        camera->position[2] + camera->front[2]
    };
    mat4LookAt(out, camera->position, target, camera->up);
}

void cameraMoveForward(Camera* camera, float deltaTime)
{
    float v = camera->movementSpeed * deltaTime;
    camera->position[0] += camera->front[0] * v;
    camera->position[1] += camera->front[1] * v;
    camera->position[2] += camera->front[2] * v;
}

void cameraMoveBackward(Camera* camera, float deltaTime)
{
    float v = camera->movementSpeed * deltaTime;
    camera->position[0] -= camera->front[0] * v;
    camera->position[1] -= camera->front[1] * v;
    camera->position[2] -= camera->front[2] * v;
}

void cameraMoveLeft(Camera* camera, float deltaTime)
{
    float v = camera->movementSpeed * deltaTime;
    camera->position[0] -= camera->right[0] * v;
    camera->position[1] -= camera->right[1] * v;
    camera->position[2] -= camera->right[2] * v;
}

void cameraMoveRight(Camera* camera, float deltaTime)
{
    float v = camera->movementSpeed * deltaTime;
    camera->position[0] += camera->right[0] * v;
    camera->position[1] += camera->right[1] * v;
    camera->position[2] += camera->right[2] * v;
}

void cameraProcessMouse(Camera* camera, float xOffset, float yOffset)
{
    camera->yaw += xOffset * camera->mouseSensitivity;
    camera->pitch += yOffset * camera->mouseSensitivity;

    /* Pitch begrenzen, damit die Kamera nicht ueberkippt */
    if (camera->pitch > 89.0f) camera->pitch = 89.0f;
    if (camera->pitch < -89.0f) camera->pitch = -89.0f;

    updateVectors(camera);
}