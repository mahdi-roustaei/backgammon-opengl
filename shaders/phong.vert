#version 330 core

/* Gleiche Vertex-Layout-Reihenfolge wie Sepehrs basic_textured.vert:
 * Position / TexCoord / Normal (passend zu seinem RenderMesh-VBO) */
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normalMatrix;

/* Outputs an Fragment-Shader (werden interpoliert) */
out vec3 fragPos;       /* Position im Welt-Koordinatensystem */
out vec3 fragNormal;    /* Normale im Welt-Koordinatensystem */
out vec2 fragTexCoord;

void main()
{
    fragPos = vec3(model * vec4(position, 1.0));
    fragNormal = normalMatrix * normal;
    fragTexCoord = texCoord;

    gl_Position = projection * view * model * vec4(position, 1.0);
}