#include "CelestialBody.h"
#include <string.h>


CelestialBody bodies[MAX_BODIES] = {0};
int count = 0;

CelestialBody CreateBody(char *name, Vector3 position, float mass, float radius){
    CelestialBody b;
    b.name = strdup(name);
    b.position = (Vector3){position.x, position.y, position.z};
    b.mass = mass;
    b.radius = radius;
    if(count < MAX_BODIES){
        bodies[count] = b;
        count++;
    }
    return b;
}

void DrawBodies() {
    for(int i = 0; i < count; i++){
        CelestialBody *body = &bodies[i];
        rlPushMatrix();
            rlTranslatef(body->position.x, body->position.y, body->position.z);
            rlScalef(body->radius, body->radius, body->radius);

            Color fill = (Color){15, 15, 15, 5};
            // DrawSphere((Vector3){0, 0, 0}, 0.5f, fill);

            Color grid = (Color){150, 150, 150, 255};

            for (int i = 1; i < 16; i++) {
                float lat    = PI * (float)i / 16.0f;
                float radius = 0.5f * sinf(lat);
                float y      = 0.5f * cosf(lat);
                DrawCircle3D((Vector3){0, y, 0}, radius, (Vector3){1, 0, 0}, 90.0f, grid);
            }

            for (int i = 0; i < 16; i++) {
                float angle = (float)i * (180.0f / 16.0f);
                DrawCircle3D((Vector3){0, 0, 0}, 0.5f, (Vector3){0, 1, 0}, angle, grid);
            }

            Color outer = (Color){180, 180, 180, 255};

            // DrawSphereWires((Vector3){0, 0, 0}, 0.505f, 6, 6, outer);
        rlPopMatrix();
    }
}
