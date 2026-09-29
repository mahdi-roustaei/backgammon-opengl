#version 330 core

layout (location = 0) in vec3 position;

uniform mat4 view;
uniform mat4 projection;

out vec3 fragDirection;

void main()
{
    /* Die Position des Wuerfels dient gleichzeitig als Richtung
     * fuer den Cubemap-Lookup im Fragment-Shader. */
    fragDirection = position;

    /* View-Matrix in eine reine Rotationsmatrix umwandeln
     * (Translation entfernen), damit die Skybox immer mit der Kamera
     * mitwandert und nie naeher/weiter wirkt. */
    mat4 rotOnlyView = mat4(mat3(view));

    vec4 pos = projection * rotOnlyView * vec4(position, 1.0);

    /* Trick: z = w setzen, damit die Tiefe immer 1.0 (=ganz hinten) ist */
    gl_Position = pos.xyww;
}