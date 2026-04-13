#include <stdio.h>
#include <stdlib.h>
#include <GLES3/gl32.h>
#include <GLFW/glfw3.h>

#include "shaders.h"

float vertices[] = {
    // fill all the screen
     0.5f,  0.5f, 1.0f,
     0.5f, -0.5f, 1.0f,
    -0.5f, -0.5f, 1.0f,
    -0.5f,  0.5f, 1.0f,
};

#define GLFW_OBJ_CHECK(OBJ) \
    if (OBJ == NULL) {      \
        glfwTerminate();    \
        exit(-1);           \
    }

int main(void) {

    if (!glfwInit()) {
        return -1;
    }
    
    // creates a window
    GLFWwindow *window = glfwCreateWindow(640, 480, "Hello, world !", NULL, NULL);
    GLFW_OBJ_CHECK(window);

    // makes the window's context the current one
    glfwMakeContextCurrent(window);

    GLuint program;
    {
        GLuint vertex = load_shader("shaders/vertex.glsl", GL_VERTEX_SHADER);
        GLuint frag = load_shader("shaders/frag.glsl", GL_FRAGMENT_SHADER);
        GLuint geometry = load_shader("shaders/geometry.glsl", GL_GEOMETRY_SHADER);
        GLuint shaders[] = { vertex, frag, geometry };
        program = create_program(shaders, sizeof(shaders));
    }

    GLuint VBO, VAO;
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &VAO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    int width, height;

    while (!glfwWindowShouldClose(window)) {
        
        // rendering
        glfwGetWindowSize(window, &width, &height);
        glViewport(0, 0, width, height);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(program);
        glBindVertexArray(VAO);
        glDrawArrays(GL_POINTS, 0, 4);
        glBindVertexArray(0);

        // swap the buffers
        glfwSwapBuffers(window);

        // poll and process events
        glfwPollEvents();
    }

    glDeleteBuffers(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glDeleteProgram(program);

    glfwTerminate();
    return 0;
}