#ifndef FLUID_H
#define FLUID_H

#include <stdbool.h>
#include <pthread.h>

#define NUM_PARTICLES 1
#define X 0
#define Y 1

typedef float float2[2];
typedef double time_seconds_t;

struct Fluid {
    // particles data
    float2 *positions;
    float2 *velocities;

    // particle access across theads
    pthread_mutex_t positions_lock;
};

struct Simulation {
    struct Fluid *fluid;

    pthread_mutex_t *running_lock;
    bool *running;

    // since it does not matter if this result is outdated 
    // and that it is not overwritten by any other thread that the simulation thread 
    // it does need a lock of his own
    time_seconds_t last_time, delta_time;
};

/**
 * creates a new fluid object
 * @note uses `initialize_fluid()` to setup the positions and velcities buffer
 */
struct Fluid create_fluid_simulation(void);

/**
 * initilize the particles positions and velocities
 * @note the positions and velocities buffers must be allocated before calling this function
 */
void initialize_fluid(struct Fluid *fluid);

/**
 * frees all the buffer of the fluid
 */
void destroy_fluid(struct Fluid *fluid);

/**
 * step by a single tiny step in the fluid simulation
 */
void fluid_step(struct Fluid *fluid, time_seconds_t delta_time);

/**
 * runs the simulation until running drops to false
 * this functions is typically run on another thread, so that is why it has the mutex lock
 * alsways returns NULL to make the compiler happy
 */
void *simulation_loop(struct Simulation *simulation);

#endif