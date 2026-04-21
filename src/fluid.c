#include "fluid.h"

#include <stdlib.h>
#include <pthread.h>
#include <malloc.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
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

float random_float(float _min, float _max) {
    float scale = (float)rand() / (float)RAND_MAX;
    return _min + scale * (_max - _min);
}

/**
 * 0 is positive
 */
int sign(float x) {
    return (x >= 0)? 1 : -1;
}

void initialize_fluid(struct Fluid *fluid) {
    for (int i = 0; i < NUM_PARTICLES; i++) {
        fluid->positions[i][X] = random_float(-9.0f, 9.0f);
        fluid->positions[i][Y] = random_float(-5.0f, 5.0f);
        fluid->velocities[i][X] = random_float(-1.0f, 1.0f);
        fluid->velocities[i][Y] = random_float(-1.f, 1.0f);
    }
}

void destroy_fluid(struct Fluid *fluid) {
    #define NO_MANGLE_FREE(PTR) \
        free((PTR));            \
        (PTR) = NULL;

    NO_MANGLE_FREE(fluid->positions);
    NO_MANGLE_FREE(fluid->velocities);
}

void fluid_step(struct Fluid *fluid, time_seconds_t delta_time) {
    pthread_mutex_trylock(&fluid->positions_lock);
    {
        // simulation goes here
        for (int i = 0; i < NUM_PARTICLES; i++) {
            fluid->velocities[i][Y] -= 10.0f * delta_time;
            fluid->positions [i][X] += fluid->velocities[i][X] * delta_time;
            fluid->positions [i][Y] += fluid->velocities[i][Y] * delta_time;

            if (fabs(fluid->positions[i][Y]) > 5) {
                fluid->positions[i][Y] = 5 * sign(fluid->positions[i][Y]);
                fluid->velocities[i][Y] *= -0.7f;
            }

            if (fabs(fluid->positions[i][X]) > 9) {
                fluid->positions[i][X] = 9 * sign(fluid->positions[i][X]);
                fluid->velocities[i][X] *= -0.7f;
            }
        }
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

        usleep(10);
    }

    return NULL;
}