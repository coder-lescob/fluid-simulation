#include "shaders.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <stdbool.h>
#include <GLES3/gl32.h>
#include <GLFW/glfw3.h>

char *file_read_all(char *path) {
    // open the file
    FILE *fd = fopen(path, "r");
    CHECK_SHADER_FILE_OPEN(fd);

    // I've choosen 256 because it is a power of two that 
    // is the closest to the size in bytes of a shader program
    size_t buf_size = 256;
    char *buf = malloc(buf_size);
    memset(buf, 0, buf_size);

    char c;
    for (size_t i = 0; (c = fgetc(fd)) != EOF; i++) {
        if (i >= buf_size - 1) {
            // out of range must allocate twice as much memory
            size_t new_buf_size = 2 * buf_size;
            char *new_buf = malloc(new_buf_size);
            memset(new_buf, 0, new_buf_size);

            // copy the old buffer to the new
            memcpy(new_buf, buf, buf_size);
            free(buf);

            buf = new_buf;
            buf_size = new_buf_size;
        }

        // fine in bounds
        buf[i] = c;
    }

    fclose(fd);

    return buf;
}

GLuint load_shader(char *path, GLenum shader_type) {
    // read all the content of the shader file
    char *shader_prog = file_read_all(path);

    // create a new shader
    GLuint shader = glCreateShader(shader_type);
    glShaderSource(shader, 1, (const GLchar *const *)&shader_prog, NULL);
    glCompileShader(shader);

    // since now it has been copied in the shader object it can be freed
    free(shader_prog);

    int success;
    char infolog[512]; // I've no clue why 512
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success) {
        // get the info log and exit
        glGetShaderInfoLog(shader, 512, NULL, infolog);
        glDeleteShader(shader);

        // print the error in red
        printf("\x1b[38;5;196m" /* sets color to red */ "%s\n", infolog);
        exit(-1);
    }

    return shader;
}

GLuint create_program(GLuint *shaders, size_t num_shaders) {
    // create a program
    GLuint prog = glCreateProgram();

    // attach all shaders
    for (size_t i = 0; i < num_shaders; i++) {
        glAttachShader(prog, shaders[i]);
    }

    // link it
    glLinkProgram(prog);

    // once linked destroy all shaders
    for (size_t i = 0; i < num_shaders; i++) {
        glDeleteShader(shaders[i]);
    }

    int success;
    char infolog[512]; // I've no clue why 512
    glGetProgramiv(prog, GL_LINK_STATUS, &success);

    if (!success) {
        // get the info log and exit
        glGetProgramInfoLog(prog, 512, NULL, infolog);
        glDeleteProgram(prog);

        // print the error in red
        printf("\x1b[38;5;196m" /* sets color to red */ "%s\n", infolog);
        exit(-1);
    }

    return prog;
}