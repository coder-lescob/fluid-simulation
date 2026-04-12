#include <stdio.h>
#include <stdlib.h>
#include <GLFW/glfw3.h>

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

    while (!glfwWindowShouldClose(window)) {
        
        // rendering
        glClear(GL_COLOR_BUFFER_BIT);

        // swap the buffers
        glfwSwapBuffers(window);

        // poll and process events
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}