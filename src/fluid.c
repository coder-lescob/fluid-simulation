#include "fluid.h"

#include <malloc.h>
#include <string.h>

struct Fluid create_fluid_simulation(void) {
    struct Fluid fluid = {
        .positions  = calloc(NUM_PARTICLES, sizeof(float2)),
        .velocities = calloc(NUM_PARTICLES, sizeof(float2)),
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
    fluid->velocities[0][Y] -= 10.0f * delta_time;
    fluid->positions [0][Y] += fluid->velocities[0][Y] * delta_time;
}