#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <unistd.h>
#include <GLES3/gl32.h>
#include <GLFW/glfw3.h>

#include "shaders.h"
#include "particle_rendering.h"
#include "text_rendering.h"
#include "fluid.h"

#define GLFW_OBJ_CHECK(OBJ) \
    if (OBJ == NULL) {      \
        glfwTerminate();    \
        exit(-1);           \
    }

#define THREAD_FUNC_T void *(*)(void *)

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

    struct Fluid fluid = create_fluid_simulation();

    int width, height;

    time_seconds_t last_time = glfwGetTime(), delta_time = 0.0;

    // running flag
    pthread_mutex_t running_lock = PTHREAD_MUTEX_INITIALIZER;
    bool running = true;

    // launch simulation thread
    struct Simulation sim = {
        .fluid          = &fluid,
        .running        = &running,
        .running_lock   = &running_lock,
    };
    pthread_t simulation_thread;
    pthread_create(&simulation_thread, NULL, (THREAD_FUNC_T)(*simulation_loop), (void *)&sim);

    while (true) {
        
        // rendering
        glfwGetWindowSize(window, &width, &height);

        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);

        // fps texts
        char fps_text_render[64] = { 0 };
        snprintf(fps_text_render, 63, "RENDER THREAD:      FRAME TIME: %d ms   FPS: %d", (int)(delta_time * 1000.0f), (int)(1 / delta_time));

        char fps_text_sim[64] = { 0 };
        snprintf(fps_text_sim, 63, "SIMULATION THREAD:  STEP TIME: %d us   SPS: %d", (int)(sim.delta_time * 1000000.0f), (int)(1 / sim.delta_time));

        pthread_mutex_lock(&fluid.positions_lock);
        {
            // render the particles
            render_particles(&particle_renderer, fluid.positions, NUM_PARTICLES * sizeof(float2), height / (float)width);
            
        }
        pthread_mutex_unlock(&fluid.positions_lock);

        // render text
        render_text(&text_renderer, fps_text_render, -width / 3.0f, height / 3.14f, width, height);
        render_text(&text_renderer, fps_text_sim, -width / 3.0f, height / 3.3f, width, height);
        
        // swap the buffers
        glfwSwapBuffers(window);

        // poll and process events
        glfwPollEvents();

        if (glfwGetKey(window, GLFW_KEY_ESCAPE)) {
            glfwSetWindowShouldClose(window, true);
        }

        // time step computation
        delta_time = glfwGetTime() - last_time;
        last_time = glfwGetTime();
        
        pthread_mutex_trylock(&running_lock);
        {
            running = !glfwWindowShouldClose(window);
            if (!running) break;
        }
        pthread_mutex_unlock(&running_lock);
    }

    // wait for the simulation thread to finish
    pthread_join(simulation_thread, NULL);
    
    // destroy the fluid
    destroy_fluid(sim.fluid);

    // destroy rendering objects
    destroy_particle_renderer(&particle_renderer);
    destroy_text_renderer(&text_renderer);
    glfwDestroyWindow(window);

    // exit
    glfwTerminate();
    return 0;
}