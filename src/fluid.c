#include "fluid.h"

#include <pthread.h>
#include <malloc.h>
#include <string.h>
#include <unistd.h>
#include <GLFW/glfw3.h>

struct Fluid create_fluid_simulation(void) {
    struct Fluid fluid = {
        .positions      = calloc(NUM_PARTICLES, sizeof(float2)),
        .velocities     = calloc(NUM_PARTICLES, sizeof(float2)),
        .positions_lock = PTHREAD_MUTEX_INITIALIZER,
    };

    // init the fluid
    initialize_fluid(&fluid);

    return fluid;
}

void initialize_fluid(struct Fluid *fluid) {
    memset(fluid->positions , 0, NUM_PARTICLES * sizeof(float2));
    memset(fluid->velocities, 0, NUM_PARTICLES * sizeof(float2));
}

void destroy_fluid(struct Fluid *fluid) {
    #define NO_MANGLE_FREE(PTR) \
        free((PTR));            \
        (PTR) = NULL;

    NO_MANGLE_FREE(fluid->positions);
    NO_MANGLE_FREE(fluid->velocities);
}

void fluid_step(struct Fluid *fluid, time_seconds_t delta_time) {
    pthread_mutex_lock(&fluid->positions_lock);
    {
        // simulation goes here
        fluid->velocities[0][Y] -= 10.0f * delta_time;
        fluid->positions [0][Y] += fluid->velocities[0][Y] * delta_time;
    }
    pthread_mutex_unlock(&fluid->positions_lock);
}

void *simulation_loop(struct Simulation *simulation) {
    // delta_time vars
    simulation->last_time = glfwGetTime(), simulation->delta_time = 0.0;

    while (true) {
        pthread_mutex_trylock(simulation->running_lock);
        {
            if (!(*simulation->running)) {
                break;
            }
        }
        pthread_mutex_unlock(simulation->running_lock);
        
        // physic simulation
        fluid_step(simulation->fluid, simulation->delta_time);

        // time step computation
        simulation->delta_time = glfwGetTime() - simulation->last_time;
        simulation->last_time  = glfwGetTime();

        usleep(100);
    }

    return NULL;
}