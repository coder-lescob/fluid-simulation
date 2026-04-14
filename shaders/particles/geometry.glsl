#version 330 core
layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

uniform vec2 window_size;
out vec2 relative_coords;
  
void main() {
    vec2 relative_square_size = vec2(0.1, 0.1) / window_size;

    relative_coords = vec2(-1, 1);
    gl_Position = gl_in[0].gl_Position + vec4(-relative_square_size.x,  relative_square_size.y, 0.0, 0.0); 
    EmitVertex();
    
    relative_coords = vec2(-1, -1);
    gl_Position = gl_in[0].gl_Position + vec4(-relative_square_size.x, -relative_square_size.y, 0.0, 0.0);
    EmitVertex();

    relative_coords = vec2(1, 1);
    gl_Position = gl_in[0].gl_Position + vec4( relative_square_size.x,  relative_square_size.y, 0.0, 0.0);
    EmitVertex();

    relative_coords = vec2(1, -1);
    gl_Position = gl_in[0].gl_Position + vec4( relative_square_size.x, -relative_square_size.y, 0.0, 0.0); 
    EmitVertex();
    
    EndPrimitive();
}