#include "CelestialBody.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#define G 10.f


CelestialBody bodies[MAX_BODIES] = {0};
int bodiesCount = 0;

CelestialBody CreateBody(char *name, Vector3 position, float mass, float radius){
    CelestialBody b;
    b.name = strdup(name);
    b.velocity = (Vector3){0, 0, 0};
    b.position = (Vector3){position.x, position.y, position.z};
    b.trace = malloc(sizeof(Vector3) * TRACE_LENGTH);
    b.mass = mass;
    b.radius = radius;
    if(bodiesCount < MAX_BODIES){
        bodies[bodiesCount] = b;
        bodiesCount++;
    }
    return b;
}

void MakeOrbit(int bIndex, int hostIndex) {
    CelestialBody *b = &bodies[bIndex];
    CelestialBody *host = &bodies[hostIndex];
    Vector3 diff = {
        host->position.x - b->position.x,
        host->position.y - b->position.y,
        host->position.z - b->position.z
    };

    float distSq = diff.x*diff.x + diff.y*diff.y + diff.z*diff.z;
    float dist   = sqrtf(distSq);

    float speed  = sqrtf((G * host->mass) / dist);

    Vector3 perp = {
        -diff.z / dist,
         0,
         diff.x / dist
    };

    b->velocity.x = perp.x * speed;
    b->velocity.y = perp.y * speed;
    b->velocity.z = perp.z * speed;
}

void DrawBodies() {
    for(int i = 0; i < bodiesCount; i++){
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

void DrawTraces() {
    for (int i = 0; i < bodiesCount; i++) {
        CelestialBody *b = &bodies[i];
        for (int j = 0; j < b->traceCount - 1; j++) {
            int a = (b->traceIndex - b->traceCount + j + TRACE_LENGTH) % TRACE_LENGTH;
            int n = (a + 1) % TRACE_LENGTH;

            float alpha = (float)j / b->traceCount;
            Color c = (Color){180, 180, 180, (unsigned char)(alpha * 180)};

            DrawLine3D(b->trace[a], b->trace[n], c);
        }
    }
}

void UpdateBodies(float dt){
    for(int i = 0; i < bodiesCount; i++){
        Vector3 acceleration = {0, 0, 0};
        for(int j = 0; j < bodiesCount; j++){
            if(i == j) continue;
            Vector3 posA = bodies[i].position;
            Vector3 posB = bodies[j].position;
            float massA = bodies[i].mass;
            float massB = bodies[j].mass;

            Vector3 diff = (Vector3){
                posB.x-posA.x,
                posB.y-posA.y,
                posB.z-posA.z
            };

            float distSq = diff.x*diff.x+diff.y*diff.y+diff.z*diff.z;
            float dist = sqrtf(distSq);
            float mag    = (G * bodies[j].mass) / distSq;

            acceleration.x += mag * (diff.x / dist);
            acceleration.y += mag * (diff.y / dist);
            acceleration.z += mag * (diff.z / dist);
        }
        bodies[i].position.x += bodies[i].velocity.x * dt + 0.5f * acceleration.x * dt * dt;
        bodies[i].position.y += bodies[i].velocity.y * dt + 0.5f * acceleration.y * dt * dt;
        bodies[i].position.z += bodies[i].velocity.z * dt + 0.5f * acceleration.z * dt * dt;

        bodies[i].velocity.x += acceleration.x * dt;
        bodies[i].velocity.y += acceleration.y * dt;
        bodies[i].velocity.z += acceleration.z * dt;

        bodies[i].trace[bodies[i].traceIndex] = bodies[i].position;
        bodies[i].traceIndex = (bodies[i].traceIndex + 1) % TRACE_LENGTH;
        if(bodies[i].traceCount < TRACE_LENGTH) bodies[i].traceCount++;
    }
}

void InitializeSolarSystem(){
    // CelestialBody sun = CreateBody("SUN", (Vector3){0, 0, 0}, 33000, 150);
    // CelestialBody earth = CreateBody("EARTH", (Vector3){1000, 0, 0}, 1, 15);
    // MakeOrbit(&bodies[1], &bodies[0]);
}
