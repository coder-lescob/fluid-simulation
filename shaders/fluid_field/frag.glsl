#version 330 core

in vec2 world_coord;
out vec4 FragColor;

#define NUM_PARTICLES 100

uniform vec2 window_size;
uniform vec2 particles_positions[NUM_PARTICLES];

#define PI 3.14159 // enough
#define MASS 1
#define SMOOTHING_RADIUS 3

float smoothing_kernel(float dst, float smoothing_radius) {
    if (dst < 0 || dst > smoothing_radius) return 0.0;

    float volume_correction = 15 / (PI * pow(smoothing_radius, 6.0)); // 15 / (PI * h^6)
    return volume_correction * pow(smoothing_radius - dst, 3.0);
}

float compute_density(vec2 pos) {

    float density = 0;

    for (int i = 0; i < NUM_PARTICLES; i++) {
        float dst = length(particles_positions[i].xy - pos);
        density += MASS * smoothing_kernel(dst, SMOOTHING_RADIUS);
    } 

    return density;
}

void main() {
    float density = compute_density(world_coord);
    FragColor = vec4(0.0, density, density, 0.0);
}
