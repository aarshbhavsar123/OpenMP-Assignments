#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

#define G 6.674e-11     
#define DT 0.01         
#define NUM_PARTICLES 1500
#define NUM_THREADS 4

typedef struct {
    double x, y;       
    double vx, vy;   
    double mass;      
} Particle;

void initialize(Particle particles[]) {
    
    #pragma omp parallel for num_threads(NUM_THREADS)
    for (int i = 0; i < NUM_PARTICLES; i++) {
        particles[i].x = rand() / (double)RAND_MAX;
        particles[i].y = rand() / (double)RAND_MAX;
        particles[i].vx = rand() / (double)RAND_MAX;
        particles[i].vy = rand() / (double)RAND_MAX;
        particles[i].mass = 1.0;
    }
}

void update(Particle particles[]) {
    #pragma omp parallel for num_threads(NUM_THREADS)
    for (int i = 0; i < NUM_PARTICLES; i++) {
        double ax = 0.0, ay = 0.0;
        for (int j = 0; j < NUM_PARTICLES; j++) {
            if (i != j) {
                double dx = particles[j].x - particles[i].x;
                double dy = particles[j].y - particles[i].y;
                double r2 = dx*dx + dy*dy;
                if (r2 > 1e-6) { 
                    double f = G * particles[i].mass * particles[j].mass / r2;
                    double theta = atan2(dy, dx);
                    ax += f / particles[i].mass * cos(theta);
                    ay += f / particles[i].mass * sin(theta);
                }
            }
        }
        particles[i].vx += ax * DT;
        particles[i].vy += ay * DT;
        particles[i].x += particles[i].vx * DT;
        particles[i].y += particles[i].vy * DT;
        if (isnan(particles[i].x) || isnan(particles[i].y) ||
            isnan(particles[i].vx) || isnan(particles[i].vy)) {
            printf("Error: NaN detected for particle %d\n", i);
        }
    }
}


int main() {
    Particle particles[NUM_PARTICLES];
    initialize(particles);

    
    for (int step = 0; step < 1000; step++) {
        update(particles);
    }
    printf("Final positions and velocities:\n");
    for (int i = 0; i < NUM_PARTICLES; i++) {
        printf("Particle %d: x=%f, y=%f, vx=%f, vy=%f\n", i, particles[i].x, particles[i].y, particles[i].vx, particles[i].vy);
    }

    return 0;
}
