#version 330 core

layout (location = 0) in vec3 pos;

uniform vec2 window_size;

void main() {
    gl_Position = vec4(pos.xy / window_size, 1.0, 1.0);
}