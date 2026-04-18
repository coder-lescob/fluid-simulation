#ifndef FLUID_H
#define FLUID_H

#define NUM_PARTICLES 1
#define X 0
#define Y 1

typedef float float2[2];

struct Fluid {
    // particles data
    float2 *positions;
    float2 *velocities;
};

#endif