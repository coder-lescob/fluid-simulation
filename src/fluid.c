#include "fluid.h"
#include "particle_rendering.h"

#include <stdlib.h>
#include <pthread.h>
#include <malloc.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <GLFW/glfw3.h>

struct Fluid create_fluid_simulation(void) {
    struct Fluid fluid = {
        .num_particles  = NUM_PARTICLES,
        .smoothing_radius = SMOOTHING_RADIUS,
        .positions      = calloc(NUM_PARTICLES, sizeof(float2)),
        .velocities     = calloc(NUM_PARTICLES, sizeof(float2)),
        .densities      = calloc(NUM_PARTICLES, sizeof(float)),
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
    for (int i = 0; i < fluid->num_particles; i++) {
        fluid->positions[i].x = random_float(-9.0f, 9.0f);
        fluid->positions[i].y = random_float(-5.0f, 5.0f);
        fluid->velocities[i].x = random_float(-1.0f, 1.0f);
        fluid->velocities[i].y = random_float(-1.f, 1.0f);
        fluid->densities[i] = 0;
    }
}

void destroy_fluid(struct Fluid *fluid) {
    #define NO_MANGLE_FREE(PTR) \
        free((PTR));            \
        (PTR) = NULL;

    NO_MANGLE_FREE(fluid->positions);
    NO_MANGLE_FREE(fluid->velocities);
}

float smoothing_kernel(float dst, float smoothing_radius) {
    if (dst < 0 || dst > smoothing_radius) return 0;

    float volum_correction = 15 / (PI * powf(smoothing_radius, 6));
    return volum_correction * powf(smoothing_radius - dst, 3);
}

float smoothing_kernel_derivative(float dst, float smoothing_radius) {
    if (dst < 0 || dst > smoothing_radius) return 0;

    float volum_correction = 15 / (PI * powf(smoothing_radius, 6));
    return volum_correction * 3 * powf(smoothing_radius - dst, 2);
}

float magnitude(float2 v) {
    return sqrtf(v.x * v.x + v.y * v.y);
}

#define SUB_VEC2(A, B) (float2){ (A).x - (B).x, (A).y - (B).y }

float compute_density(struct Fluid *fluid, int particle_index) {

    // TODO: optimize this to only look for particles in the smoothing radius

    const float mass = 1;
    float density = 0;

    for (int i = 0; i < fluid->num_particles; i++) {
        float dst = magnitude(SUB_VEC2(fluid->positions[i], fluid->positions[particle_index]));
        density += mass * smoothing_kernel(dst, fluid->smoothing_radius);
    }

    return density;
}

float get_pressure(float density) {
    #define TARGET_DENSITY 1.0f
    #define PRESSURE_MULTIPLER 10.0f

    float error = TARGET_DENSITY - density;
    return PRESSURE_MULTIPLER * -error;
}

float get_shared_pressure(float density_a, float density_b) {
    return (get_pressure(density_a) + get_pressure(density_b)) / 2.0f;
}

float2 compute_pressure_force(struct Fluid *fluid, int particle_index) {

    const float mass = 1;
    float2 pressure_force = (float2){ 0, 0 };

    for (int i = 0; i < fluid->num_particles; i++) {
        if (i == particle_index) continue;
        float2 offset = SUB_VEC2(fluid->positions[i], fluid->positions[particle_index]);
        float  dst    = magnitude(offset);
        float2 dir = { 0, 0 };
        if (dst == 0) {
            // TODO: pick a random direction
            continue;
        }
        else {
            dir = (float2){ offset.x / dst, offset.y / dst };
        }

        pressure_force.x += mass * dir.x * get_shared_pressure(fluid->densities[i], fluid->densities[particle_index]) * smoothing_kernel_derivative(dst, fluid->smoothing_radius);
        pressure_force.y += mass * dir.y * get_shared_pressure(fluid->densities[i], fluid->densities[particle_index]) * smoothing_kernel_derivative(dst, fluid->smoothing_radius);
    }

    return pressure_force;
}

void fluid_step(struct Fluid *fluid, time_seconds_t delta_time) {
    pthread_mutex_trylock(&fluid->positions_lock);
    {
        for (int i = 0; i < NUM_PARTICLES; i++) {
            fluid->densities[i] = compute_density(fluid, i);
        }

        // simulation goes here
        for (int i = 0; i < NUM_PARTICLES; i++) {
            float2 pressure_force = compute_pressure_force(fluid, i);

            fluid->velocities[i].x += pressure_force.x / fluid->densities[i] * delta_time;
            fluid->velocities[i].y += pressure_force.y / fluid->densities[i] * delta_time;
        }

            //fluid->velocities[i].y -= 10.0f * delta_time;
        for (int i = 0; i < NUM_PARTICLES; i++) {
            fluid->positions [i].x += fluid->velocities[i].x * delta_time;
            fluid->positions [i].y += fluid->velocities[i].y * delta_time;

            if (fabs(fluid->positions[i].y) > SCREEN_WIDTH_METERS * (9.0f / 16.0f) - PARTICLE_SIZE) {
                fluid->positions[i].y = (SCREEN_WIDTH_METERS * (9.0f / 16.0f) - PARTICLE_SIZE) * sign(fluid->positions[i].y);
                fluid->velocities[i].y *= -1;//0.7f;
            }

            if (fabs(fluid->positions[i].x) > SCREEN_WIDTH_METERS - PARTICLE_SIZE) {
                fluid->positions[i].x = (SCREEN_WIDTH_METERS - PARTICLE_SIZE) * sign(fluid->positions[i].x);
                fluid->velocities[i].x *= -1;//0.7f;
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