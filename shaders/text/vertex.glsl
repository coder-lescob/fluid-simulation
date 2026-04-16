#version 330 core

layout (location = 0) in vec2 pos;
layout (location = 1) in vec2 uvs;

uniform vec2 window_size;
out vec2 uv;

void main() {
    gl_Position = vec4(pos.xy / window_size, 1.0, 1.0);
    uv = uvs;
}