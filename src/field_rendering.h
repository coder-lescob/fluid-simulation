#ifndef FIELD_H
#define FIELD_H

#include <GLES3/gl32.h>

#include "fluid.h"

struct FieldRenderObject {
    GLuint program;
    GLuint VAO, VBO;
};

/**
 * creates a field renderer
 */
struct FieldRenderObject create_field_renderer(void);

/**
 * renders the fluid field of the given fluid
 */
void render_fluid_field(struct FieldRenderObject *renderer, struct Fluid *fluid, float inv_aspect_ratio);

/**
 * destroys the field renderer given
 */
void destroy_field_renderer(struct FieldRenderObject *renderer);

#endif