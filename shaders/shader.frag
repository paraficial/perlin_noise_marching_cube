#version 330 core

in vec3 position;
in vec3 normal;

const vec3 cameraPosition = vec3(0.0, 0.0, 6.0);
const vec3 lightPosition = vec3(0.0, 10.0, 10.0);
const vec3 lightColor = vec3(1.0, 1.0, 1.0);

out vec4 color_out_0;

void main() {
    float ambientStrength = 0.2;
    vec3 ambient = ambientStrength * lightColor;

    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightPosition - position);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    float specularStrength = 0.4;
    vec3 viewDir = normalize(cameraPosition - position);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;

    vec3 objectColor = vec3(0.0, 1.0, 0.5);
    vec3 result = (ambient + diffuse + specular) * (objectColor);

    color_out_0 = vec4(clamp(result, 0, 1), 1.0);
//    color_out_0 = vec4(1.0);
}
