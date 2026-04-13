#ifndef SHADERS_H
#define SHADERS_H

#include <GLFW/glfw3.h>

#define CHECK_SHADER_FILE_OPEN(FD)              \
    if (FD == NULL) {                           \
        perror("shader file failed to open");   \
        exit(-1);                               \
    }

/**
 * read all the content of a file to string
 * @note the string returned MUST be freed using `free()`
 */
char *file_read_all(char *path);

/**
 * Loads a shader from a file
 */
GLuint load_shader(char *path, GLenum shader_type);

/**
 * creates a new program from all shaders given
 */
GLuint create_program(GLuint *shaders, size_t num_shaders);

#endif