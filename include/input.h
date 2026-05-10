#ifndef INPUT_H
#define INPUT_H
#include "region.h"
#include <raylib.h>
#include <math.h>

void Input(Region *simRegion, Camera3D *camera);
Vector3 GetMouseWorldPos(Camera3D *camera, Region *r);

#endif
