#include "particle_rendering.h"

#include "shaders.h"

struct ParticleRenderObject create_particle_renderer(size_t num_particles) {

    // loads and compile the shaders
    GLuint program;
    {
        GLuint vertex = load_shader("shaders/particles/vertex.glsl", GL_VERTEX_SHADER);
        GLuint frag = load_shader("shaders/particles/frag.glsl", GL_FRAGMENT_SHADER);
        GLuint geometry = load_shader("shaders/particles/geometry.glsl", GL_GEOMETRY_SHADER);
        GLuint shaders[] = { vertex, frag, geometry };
        program = create_program(shaders, sizeof(shaders) / sizeof(GLuint));
    }

    // create Vertex Buffer Object and Vertex Array Object
    GLuint VBO, VAO;
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, num_particles * 2 * sizeof(float), NULL, GL_DYNAMIC_DRAW);

    // allocate to the right shader buffer the VBO
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    return (struct ParticleRenderObject) {
        .program = program,
        .VAO = VAO,
        .VBO = VBO,
    };
}

void render_particles(struct ParticleRenderObject *particles_renderer, float *particles_positions, GLsizeiptr positions_size, float inv_aspect_ratio) {
    glUseProgram(particles_renderer->program);
    glBindVertexArray(particles_renderer->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, particles_renderer->VBO);
    {
        glBufferSubData(GL_ARRAY_BUFFER, 0, positions_size, particles_positions);
        glUniform2f(glGetUniformLocation(particles_renderer->program, "window_size"), SCREEN_WIDTH_METERS, SCREEN_WIDTH_METERS * inv_aspect_ratio);
        glUniform1f(glGetUniformLocation(particles_renderer->program, "particle_size"), PARTICLE_SIZE);
        glDrawArrays(GL_POINTS, 0, positions_size / (2 * sizeof(float)));
    }
    glBindVertexArray(0);
}

void destroy_particle_renderer(struct ParticleRenderObject *particle_renderer) {
    // delete buffers
    glDeleteVertexArrays(1, &particle_renderer->VAO);
    glDeleteBuffers(1, &particle_renderer->VBO);

    // delete program
    glDeleteProgram(particle_renderer->program);
}