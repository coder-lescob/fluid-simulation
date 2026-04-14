#include "particle_rendering.h"

#include "shaders.h"

struct ParticleRenderObject create_particle_renderer(void) {

    // loads and compile the shaders
    GLuint program;
    {
        GLuint vertex = load_shader("shaders/particles/vertex.glsl", GL_VERTEX_SHADER);
        GLuint frag = load_shader("shaders/particles/frag.glsl", GL_FRAGMENT_SHADER);
        GLuint geometry = load_shader("shaders/particles/geometry.glsl", GL_GEOMETRY_SHADER);
        GLuint shaders[] = { vertex, frag, geometry };
        program = create_program(shaders, sizeof(shaders));
    }

    // create Vertex Buffer Object and Vertex Array Object
    GLuint VBO, VAO;
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &VAO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // allocate to the right shader buffer the VBO
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    return (struct ParticleRenderObject) {
        .program = program,
        .VAO = VAO,
        .VBO = VBO,
    };
}

void render_particles(struct ParticleRenderObject *particles_renderer, float *particles_positions, GLsizeiptr positions_size, float inv_aspect_ratio) {
    glUseProgram(particles_renderer->program);
    glBindVertexArray(particles_renderer->VAO);
    {
        glBufferData(GL_ARRAY_BUFFER, positions_size, particles_positions, GL_DYNAMIC_READ);
        glUniform2f(glGetUniformLocation(particles_renderer->program, "window_size"), SCREEN_WIDTH_METERS, SCREEN_WIDTH_METERS * inv_aspect_ratio);
        glUniform1f(glGetUniformLocation(particles_renderer->program, "particle_size"), PARTICLE_SIZE);
        glDrawArrays(GL_POINTS, 0, positions_size / (3 * sizeof(float)));
    }
    glBindVertexArray(0);
}

void destroy_particle_renderer(struct ParticleRenderObject *particle_renderer) {
    // delete buffers
    glDeleteBuffers(1, &particle_renderer->VAO);
    glDeleteBuffers(1, &particle_renderer->VBO);

    // delete program
    glDeleteProgram(particle_renderer->program);
}