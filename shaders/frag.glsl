#version 330 core

in vec2 relative_coords;
out vec4 FragColor;

void main() {
    // purple
    if (relative_coords.x * relative_coords.x + relative_coords.y * relative_coords.y > 1) {
        discard;
    }
    
    FragColor = vec4(0.0, 0.55, 0.75, 0.0);
} 