#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <GLES3/gl32.h>
#include <GLFW/glfw3.h>

#include "shaders.h"
#include "particle_rendering.h"
#include "text_rendering.h"

float particles_positions[] = {
     5.0f,  5.0f, 1.0f,
     5.0f, -5.0f, 1.0f,
    -5.0f, -5.0f, 1.0f,
    -5.0f,  5.0f, 1.0f,
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
    GLFWwindow *window = glfwCreateWindow(480, 480, "fluid simulation", glfwGetPrimaryMonitor(), NULL);
    GLFW_OBJ_CHECK(window);

    // makes the window's context the current one
    glfwMakeContextCurrent(window);

    struct ParticleRenderObject particle_renderer = 
        create_particle_renderer(4);

    struct TextRenderObject text_renderer = 
        create_text_renderer();

    int width, height;

    while (!glfwWindowShouldClose(window)) {
        
        // rendering
        glfwGetWindowSize(window, &width, &height);

        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);

        // render the particles
        render_particles(&particle_renderer, particles_positions, sizeof(particles_positions), height / (float)width);
        render_text(&text_renderer, "Ok now it is fluid time.", 0, 0, width, height);

        // swap the buffers
        glfwSwapBuffers(window);

        // poll and process events
        glfwPollEvents();

        if (glfwGetKey(window, GLFW_KEY_ESCAPE)) {
            glfwSetWindowShouldClose(window, true);
        }

        particles_positions[1] -= 0.05f;
        particles_positions[4] += 0.05f;
        particles_positions[7] -= 0.05f;
        particles_positions[10] += 0.05f; 
    }

    destroy_particle_renderer(&particle_renderer);
    destroy_text_renderer(&text_renderer);
    glfwDestroyWindow(window);

    glfwTerminate();
    return 0;
}