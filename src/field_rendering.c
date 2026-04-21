#include "field_rendering.h"
#include "particle_rendering.h"
#include "shaders.h"

struct FieldRenderObject create_field_renderer(void) {
    // loads and compile the shaders
    GLuint program;
    {
        GLuint vertex = load_shader("shaders/fluid_field/vertex.glsl", GL_VERTEX_SHADER);
        GLuint frag = load_shader("shaders/fluid_field/frag.glsl", GL_FRAGMENT_SHADER);
        GLuint shaders[] = { vertex, frag };
        program = create_program(shaders, sizeof(shaders) / sizeof(GLuint));
    }

    // create Vertex Buffer Object and Vertex Array Object
    GLuint VBO, VAO;
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);

    float2 screen_viewport[4] = {
        {  1.0f,  1.0f },
        { -1.0f,  1.0f },
        {  1.0f, -1.0f },
        { -1.0f, -1.0f },
    };

    glBindVertexArray(VAO);
    {
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(screen_viewport), screen_viewport, GL_STATIC_DRAW);

        // allocate to the right shader buffer the VBO
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float2), (void *)0);
        glEnableVertexAttribArray(0);
    }
    glBindVertexArray(0);

    return (struct FieldRenderObject) {
        .program = program,
        .VAO = VAO,
        .VBO = VBO,
    };
}

void render_fluid_field(struct FieldRenderObject *renderer, struct Fluid *fluid, float inv_aspect_ratio) {
    glUseProgram(renderer->program);
    glBindVertexArray(renderer->VAO);
    {
        // send the window size
        glUniform2f(glGetUniformLocation(renderer->program, "window_size"), SCREEN_WIDTH_METERS, SCREEN_WIDTH_METERS * inv_aspect_ratio);

        // send the position buffer
        pthread_mutex_trylock(&fluid->positions_lock);
        {
            glUniform2fv(glGetUniformLocation(renderer->program, "particles_positions"), NUM_PARTICLES, (const GLfloat *)fluid->positions);
        }
        pthread_mutex_unlock(&fluid->positions_lock);

        // draw
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }
    glBindVertexArray(0);
}

void destroy_field_renderer(struct FieldRenderObject *renderer) {
    // delete buffers
    glDeleteVertexArrays(1, &renderer->VAO);
    glDeleteBuffers(1, &renderer->VBO);

    // delete program
    glDeleteProgram(renderer->program);
}