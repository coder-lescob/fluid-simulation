#version 420 core
layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

// the number of vertices in a square
#define NUM_CORNERS 4

uniform vec2 window_size;
uniform float particle_size;
out vec2 relative_coords;
  
void main() {
    vec2 relative_square_size = particle_size / window_size;

    vec2 corners[NUM_CORNERS] = {
        vec2(-1,  1),
        vec2(-1, -1),
        vec2( 1,  1),
        vec2( 1, -1),
    };

    for (int i = 0; i < NUM_CORNERS; i++) {
        relative_coords = corners[i];
        gl_Position = gl_in[0].gl_Position + vec4(relative_coords * relative_square_size, 0.0, 0.0); 
        EmitVertex();
    }
    EndPrimitive();
}