#version 330 core

layout (location = 0) in vec3 position_in;
layout (location = 1) in vec3 normal_in;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out vec3 position;
out vec3 normal;

void main() {
    gl_Position = projection * view * model * vec4(position_in, 1);
    position = vec3(projection * view * model * vec4(position_in, 1));
    normal = vec3(transpose(inverse(view * model)) * vec4(normal_in, 1));
}
