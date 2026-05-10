#ifndef MINIVIEWPORT_H
#define MINIVIEWPORT_H
#include <raylib.h>
#include "region.h"
#include "CelestialBody.h"


typedef struct{
    Region r;
    Camera3D camera;
    float angle;
} MiniViewport;

MiniViewport CreateMiniViewport(float x, float y, float w, float h);

void UpdateMiniViewport(MiniViewport *mv);
void DrawMiniViewport(MiniViewport *mv, CelestialBody *body);

#endif
