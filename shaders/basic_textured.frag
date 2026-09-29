#version 330 core

in vec2 fragTexCoord;
in vec3 fragNormal;

out vec4 color;

uniform sampler2D diffuseTexture;
uniform float objectAlpha;

void main()
{
    vec3 lightDirection = normalize(vec3(0.5, 1.0, 0.3));
    vec3 normalDirection = normalize(fragNormal);

    float diffuse = max(dot(normalDirection, lightDirection), 0.0);
    vec3 baseColor = texture(diffuseTexture, fragTexCoord).rgb;

    vec3 ambientColor = 0.25 * baseColor;
    vec3 diffuseColor = diffuse * baseColor;

    color = vec4(ambientColor + diffuseColor, objectAlpha);
}
