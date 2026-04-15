#ifndef TEXT_RENDERING_H
#define TEXT_RENDERING_H

#include <GLES3/gl32.h>

#define CHAR_WIDTH 8
#define CHAR_HEIGHT 13

struct TextRenderObject {
    GLuint program;
    GLuint VAO, VBO;
};

/**
 * creates a new text renderer
 */
struct TextRenderObject create_text_renderer(void);

/**
 * renders a text starting a (x;y)
 */
void render_text(struct TextRenderObject *text_renderer, char *text, float x, float y, int window_width, int window_height);

/**
 * destroys the text renderer
 */
void destroy_text_renderer(struct TextRenderObject *text_renderer);

#endif