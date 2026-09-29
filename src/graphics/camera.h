#ifndef CAMERA_H
#define CAMERA_H

/*
 * Kamera-Struktur fuer eine 3D-Szene.
 * Wir verwenden eine "fly camera": die Kamera bewegt sich frei im Raum,
 * gesteuert ueber yaw (links/rechts schauen) und pitch (hoch/runter).
 */
typedef struct
{
    float position[3];      /* Position der Kamera im Welt-Koordinatensystem */
    float front[3];         /* Blickrichtung (normiert) */
    float up[3];            /* "Oben"-Vektor (normiert) */
    float right[3];         /* Rechts-Vektor (normiert, ergibt sich aus front und up) */
    float worldUp[3];       /* Globale Oben-Richtung, meistens (0, 1, 0) */

    float yaw;              /* Rotation um die Y-Achse (Grad), 0 = nach -Z schauen */
    float pitch;            /* Rotation um die X-Achse (Grad), -89..89 begrenzt */

    float movementSpeed;    /* Einheiten pro Sekunde */
    float mouseSensitivity; /* Grad pro Pixel Mausbewegung */
} Camera;

/*
 * Initialisiert die Kamera an einer Startposition.
 * Standardausrichtung: Blick entlang der negativen Z-Achse.
 */
void cameraInit(Camera* camera, float posX, float posY, float posZ);

/*
 * Berechnet die View-Matrix der Kamera (intern mit mat4LookAt).
 * Das Ergebnis wird in 'out' geschrieben (16 floats, column-major).
 */
void cameraGetViewMatrix(const Camera* camera, float* out);

/* Bewegung: deltaTime in Sekunden, damit die Geschwindigkeit framerate-unabhaengig ist */
void cameraMoveForward(Camera* camera, float deltaTime);
void cameraMoveBackward(Camera* camera, float deltaTime);
void cameraMoveLeft(Camera* camera, float deltaTime);
void cameraMoveRight(Camera* camera, float deltaTime);

/* Reagiert auf Mausbewegung (xOffset/yOffset in Pixeln seit dem letzten Frame) */
void cameraProcessMouse(Camera* camera, float xOffset, float yOffset);

#endif