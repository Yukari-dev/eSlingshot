#ifndef CELESTIAL_BODY_H
#define CELESTIAL_BODY_H
#define MAX_BODIES 16
#define TRACE_LENGTH 1500
#define G 10.f
#include <raylib.h>
#include <math.h>
#include "rlgl.h"
#include "region.h"

typedef enum {
    BODY_STAR,
    BODY_GAS_GIANT,
    BODY_TERRESTRIAL,
    BODY_ICE_GIANT,
    BODY_DWARF
} BodyType;

typedef struct {
    char *name;
    Vector3 position, velocity;
    float angle, angularVelocity;
    float mass, radius;
    Vector3 *trace;
    int traceIndex, traceCount;
    BodyType bodyType;
} CelestialBody;

extern CelestialBody bodies[MAX_BODIES];
extern int bodiesCount;

CelestialBody CreateBody(char *name, Vector3 position, float mass, float radius, float angularVelocity, BodyType bodyType);

void InitializeSolarSystem();

void MakeOrbit(int bIndex, int hostIndex);

void DrawBodies();
void DrawTraces();
void DrawBodyHovered(CelestialBody *b);
void DrawHoveredBodies(Camera3D *cam, Region *r);

void UpdateBodies(float dt);

Vector3 GetMouseWorldPos(Camera3D *camera, Region *r);

bool IsMouseOverBody(Camera3D *camera, Region *r, CelestialBody *body);
bool IsMouseOverAnyBody(Camera3D *camera, Region *r);

char *GetBodyTypeName(CelestialBody *b);

#endif
