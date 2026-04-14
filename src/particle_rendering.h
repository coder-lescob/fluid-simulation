#ifndef PARTICLE_RENDERING_H
#define PARTICLE_RENDERING_H

#include <GLES3/gl32.h>
#include <GLFW/glfw3.h>

struct ParticleRenderObject {
    GLuint program;
    GLuint VAO, VBO;
};

/**
 * creates then returns a particle render object for particle rendering
 */
struct ParticleRenderObject create_particle_renderer(void);

/**
 * renders the particles from a buffer
 * @note the positions_size is size in BYTES of the positions buffer
 */
void render_particles(struct ParticleRenderObject *particles_renderer, float *particles_positions, GLsizeiptr positions_size, float aspect_ratio);

/**
 * destroys all buffers and program of the particles renderer
 */
void destroy_particle_renderer(struct ParticleRenderObject *particle_renderer);

#endif