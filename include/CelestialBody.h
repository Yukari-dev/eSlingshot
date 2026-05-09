#ifndef CELESTIAL_BODY_H
#define CELESTIAL_BODY_H
#define MAX_BODIES 16
#define TRACE_LENGTH 1500
#include <raylib.h>
#include <math.h>
#include "rlgl.h"

typedef struct {
    char *name;
    Vector3 position;
    Vector3 velocity;
    float mass;
    float radius;
    Vector3 *trace;
    int traceIndex;
    int traceCount;
} CelestialBody;

extern CelestialBody bodies[MAX_BODIES];
extern int bodiesCount;

CelestialBody CreateBody(char *name, Vector3 position, float mass, float radius);

void InitializeSolarSystem();

void MakeOrbit(int bIndex, int hostIndex);

void DrawBodies();
void DrawTraces();

void UpdateBodies(float dt);

#endif
