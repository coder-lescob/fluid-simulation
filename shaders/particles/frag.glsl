#version 330 core

in vec2 relative_coords;
out vec4 FragColor;

void main() {
    // the pythagoran theorem
    float sqr_radius = relative_coords.x * relative_coords.x + relative_coords.y * relative_coords.y;

    if (sqr_radius > 1 /* 1² == 1 */) {
        discard;
    }

    // purple
    FragColor = vec4(0.0, 0.55, 0.75, 0.0);
} 