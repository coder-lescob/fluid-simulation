#include "text_rendering.h"

#include <GLES3/gl32.h>
#include "shaders.h"

struct TextRenderObject create_text_renderer(void) {
    // loads and compile the shaders
    GLuint program;
    {
        GLuint vertex = load_shader("shaders/text/vertex.glsl", GL_VERTEX_SHADER);
        GLuint frag = load_shader("shaders/text/frag.glsl", GL_FRAGMENT_SHADER);
        GLuint shaders[] = { vertex, frag };
        program = create_program(shaders, sizeof(shaders) / sizeof(GLuint));
    }

    // create Vertex Buffer Object and Vertex Array Object
    GLuint VBO, VAO;
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);
    {
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, 16 * sizeof(float), NULL, GL_DYNAMIC_DRAW);

        // allocate to the right shader buffer the VBO
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);
        
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)(2 * sizeof(float)));
        glEnableVertexAttribArray(1);
    }
    glBindVertexArray(0);

    return (struct TextRenderObject) {
        .program = program,
        .VAO = VAO,
        .VBO = VBO,
    };
}

void move_char_box(float *char_box) {
    for (int i = 0; i < 16; i += 4) {
        char_box[i] += CHAR_WIDTH + 2;
    }
}

void render_text(struct TextRenderObject *text_renderer, char *text, float x, float y, int window_width, int window_height) {
    float char_box[] = {
        x               , y               ,      0.0f, 0.0f,
        x + CHAR_WIDTH  , y               ,      1.0f, 0.0f,
        x               , y + CHAR_HEIGHT ,      0.0f, 1.0f,
        x + CHAR_WIDTH  , y + CHAR_HEIGHT ,      1.0f, 1.0f,
    };

    glUseProgram(text_renderer->program);
    glBindVertexArray(text_renderer->VAO);
    {
        glBindBuffer(GL_ARRAY_BUFFER, text_renderer->VBO);
        glUniform2f(glGetUniformLocation(text_renderer->program, "window_size"), window_width / 3.0f, window_height / 3.0f);

        for (char *c = text; *c != 0; c++ /* ;) */) {

            // render only visible characters
            if (*c >= '!' && *c <= '~') {
                glUniform1i(glGetUniformLocation(text_renderer->program, "char"), *c);
                glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(char_box), char_box);
                glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
            }

            move_char_box(char_box);
        }
    }
    glBindVertexArray(0);
}

void destroy_text_renderer(struct TextRenderObject *text_renderer) {
    glDeleteBuffers(1, &text_renderer->VAO);
    glDeleteVertexArrays(1, &text_renderer->VBO);

    glDeleteProgram(text_renderer->program);
}