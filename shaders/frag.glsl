#version 330 core

in vec2 uvs;
out vec4 FragColor;

void main() {
    // purple
    FragColor = vec4(uvs.xy, 0.0, 0.0);
} 