#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <GLES3/gl32.h>
#include <GLFW/glfw3.h>

#include "shaders.h"
#include "particle_rendering.h"
#include "text_rendering.h"
#include "fluid.h"

float2 particles_positions[NUM_PARTICLES] = {
    {0.0f, 0.0f},
};

float2 particles_velocities[NUM_PARTICLES] = {
    {0.0f, 0.0f},
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
        create_particle_renderer(NUM_PARTICLES);

    struct TextRenderObject text_renderer = 
        create_text_renderer();

    int width, height;

    double last_time = glfwGetTime(), delta_time = 0.0;

    while (!glfwWindowShouldClose(window)) {
        
        // rendering
        glfwGetWindowSize(window, &width, &height);

        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);

        char fps_text[64] = "";
        snprintf(fps_text, 64, "FRAME: %d ms", (int)(delta_time * 1000.0f));

        // render the particles
        render_particles(&particle_renderer, particles_positions, sizeof(particles_positions), height / (float)width);
        render_text(&text_renderer, fps_text, -width / 3.0f, height / 3.14f, width, height);

        // swap the buffers
        glfwSwapBuffers(window);

        // poll and process events
        glfwPollEvents();

        if (glfwGetKey(window, GLFW_KEY_ESCAPE)) {
            glfwSetWindowShouldClose(window, true);
        }

        particles_velocities[0][Y] -= 10.0 * delta_time;
        particles_positions[0][Y] += particles_velocities[0][Y] * delta_time;

        // time step computation
        delta_time = glfwGetTime() - last_time;
        last_time = glfwGetTime();
    }

    destroy_particle_renderer(&particle_renderer);
    destroy_text_renderer(&text_renderer);
    glfwDestroyWindow(window);

    glfwTerminate();
    return 0;
}