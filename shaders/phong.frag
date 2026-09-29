#version 330 core

in vec3 fragPos;
in vec3 fragNormal;
in vec2 fragTexCoord;

out vec4 color;

/* Textur */
uniform sampler2D diffuseTexture;

/* Material */
uniform vec3 ambientColor;
uniform vec3 specularColor;
uniform float shininess;

/* Zwei Lichtquellen - erfuellt "mehr als eine Lichtquelle" fuer gute Note */
uniform vec3 light1Pos;
uniform vec3 light1Color;
uniform vec3 light2Pos;
uniform vec3 light2Color;

/* Kameraposition - fuer Specular-Berechnung */
uniform vec3 viewPos;

/* Nebel - erfuellt "Nebel in einer komplexen 3D-Szene" */
uniform vec3 fogColor;
uniform float fogStart;
uniform float fogEnd;

/* Berechnet Phong-Beleuchtung fuer EINE Lichtquelle */
vec3 calcLight(vec3 lightPos, vec3 lightColor, vec3 norm, vec3 viewDir, vec3 texColor)
{
    vec3 lightDir = normalize(lightPos - fragPos);

    /* Diffuse (Lambert) */
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor * texColor;

    /* Specular (Phong) */
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = spec * specularColor * lightColor;

    return diffuse + specular;
}

void main()
{
    vec3 texColor = texture(diffuseTexture, fragTexCoord).rgb;
    vec3 norm = normalize(fragNormal);
    vec3 viewDir = normalize(viewPos - fragPos);

    /* Ambient (einmal global) */
    vec3 ambient = ambientColor * texColor;

    /* Beide Lichtquellen addieren */
    vec3 light1 = calcLight(light1Pos, light1Color, norm, viewDir, texColor);
    vec3 light2 = calcLight(light2Pos, light2Color, norm, viewDir, texColor);

    vec3 result = ambient + light1 + light2;

    /* Nebel: linear interpolieren basierend auf Distanz zur Kamera */
    float distance = length(viewPos - fragPos);
    float fogFactor = clamp((fogEnd - distance) / (fogEnd - fogStart), 0.0, 1.0);
    result = mix(fogColor, result, fogFactor);

    color = vec4(result, 1.0);
}