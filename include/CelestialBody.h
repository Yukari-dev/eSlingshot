#ifndef CELESTIAL_BODY_H
#define CELESTIAL_BODY_H
#define MAX_BODIES 16
#include <raylib.h>
#include <math.h>
#include "rlgl.h"

typedef struct {
    char *name;
    Vector3 position;
    Vector3 velocity;
    float mass;
    float radius;
} CelestialBody;

extern CelestialBody bodies[MAX_BODIES];
extern int count;

CelestialBody CreateBody(char *name, Vector3 position, float mass, float radius);

void DrawBodies();

void UpdateBodies(float dt);

#endif
