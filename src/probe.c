#include "probe.h"
#include "probes.h"
#include "CelestialBody.h"
#include <stdlib.h>
#include <string.h>

Probe *CreateProbe(Vector3 position, Vector3 *dragStart) {
    Probe *probe      = malloc(sizeof(Probe));
    probe->position   = position;
    probe->velocity   = (Vector3){0, 0, 0};
    probe->active     = true;
    probe->launched   = false;
    probe->trace      = malloc(sizeof(Vector3) * TRACE_LENGTH);
    probe->traceIndex = 0;
    probe->traceCount = 0;
    probe->historyIndex = 0;
    probe->historyCount = 0;
    memset(probe->speedHistory, 0, sizeof(probe->speedHistory));
    return probe;
}

void DrawProbe(Probe *p){
    if(!p->active) return;
    DrawSphere(p->position, 1.f, WHITE);
}


void DrawProbeTraces(Probe *probe) {
    for (int j = 0; j < probe->traceCount - 1; j++) {
        int a = (probe->traceIndex - probe->traceCount + j + TRACE_LENGTH) % TRACE_LENGTH;
        int n = (a + 1) % TRACE_LENGTH;

        float alpha = (float)j / probe->traceCount;
        Color c = (Color){180, 180, 180, (unsigned char)(alpha * 180)};

        DrawLine3D(probe->trace[a], probe->trace[n], c);
    }
}

void UpdateProbe(Probe *p, float dt){
    if(!p->active || !p->launched) return;

    Vector3 acceleration = {0, 0, 0};
    for(int i = 0; i < bodiesCount; i++){
        Vector3 diff = (Vector3){
            bodies[i].position.x - p->position.x,
            bodies[i].position.y - p->position.y,
            bodies[i].position.z - p->position.z
        };
        float distSq = diff.x*diff.x+diff.y*diff.y+diff.z*diff.z;
        float dist = sqrtf(distSq);
        float mag = (G * bodies[i].mass) / distSq;
        acceleration.x += mag * (diff.x / dist);
        acceleration.y += mag * (diff.y / dist);
        acceleration.z += mag * (diff.z / dist);
    }
    p->position.x += p->velocity.x * dt + 0.5f * acceleration.x * dt * dt;
    p->position.y += p->velocity.y * dt + 0.5f * acceleration.y * dt * dt;
    p->position.z += p->velocity.z * dt + 0.5f * acceleration.z * dt * dt;
    p->velocity.x += acceleration.x * dt;
    p->velocity.y += acceleration.y * dt;
    p->velocity.z += acceleration.z * dt;
    p->trace[p->traceIndex] = p->position;
    p->traceIndex = (p->traceIndex + 1) % TRACE_LENGTH;
    if (p->traceCount < TRACE_LENGTH) p->traceCount++;

    float speed = sqrtf(
        p->velocity.x * p->velocity.x +
        p->velocity.y * p->velocity.y +
        p->velocity.z * p->velocity.z
    );
    p->speedHistory[p->historyIndex] = speed;
    p->historyIndex = (p->historyIndex + 1) % SPEED_HISTORY;
    if (p->historyCount < SPEED_HISTORY) p->historyCount++;
}

void PredictTrajectory(Probe *p, Vector3 initVel, Vector3 *out, int *outCount) {
    CelestialBody ghost[MAX_BODIES];
    memcpy(ghost, bodies, sizeof(CelestialBody) * bodiesCount);

    for (int i = 0; i < bodiesCount; i++)
        ghost[i].trace = NULL;

    Vector3 pos = p->position;
    Vector3 vel = initVel;
    float dt    = 0.5f;
    *outCount   = 0;

    for (int step = 0; step < PREDICT_STEPS; step++) {
        Vector3 acceleration = {0, 0, 0};

        for (int j = 0; j < bodiesCount; j++) {
            Vector3 diff = {
                ghost[j].position.x - pos.x,
                ghost[j].position.y - pos.y,
                ghost[j].position.z - pos.z
            };
            float distSq = diff.x*diff.x + diff.y*diff.y + diff.z*diff.z;
            float dist   = sqrtf(distSq);
            float mag    = (G * ghost[j].mass) / distSq;
            acceleration.x += mag * (diff.x / dist);
            acceleration.y += mag * (diff.y / dist);
            acceleration.z += mag * (diff.z / dist);
        }

        pos.x += vel.x * dt + 0.5f * acceleration.x * dt * dt;
        pos.y += vel.y * dt + 0.5f * acceleration.y * dt * dt;
        pos.z += vel.z * dt + 0.5f * acceleration.z * dt * dt;
        vel.x += acceleration.x * dt;
        vel.y += acceleration.y * dt;
        vel.z += acceleration.z * dt;

        // advance ghost bodies too
        for (int i = 0; i < bodiesCount; i++) {
            Vector3 acc = {0, 0, 0};
            for (int j = 0; j < bodiesCount; j++) {
                if (i == j) continue;
                Vector3 diff = {
                    ghost[j].position.x - ghost[i].position.x,
                    ghost[j].position.y - ghost[i].position.y,
                    ghost[j].position.z - ghost[i].position.z
                };
                float distSq = diff.x*diff.x + diff.y*diff.y + diff.z*diff.z;
                float dist   = sqrtf(distSq);
                float mag    = (G * ghost[j].mass) / distSq;
                acc.x += mag * (diff.x / dist);
                acc.y += mag * (diff.y / dist);
                acc.z += mag * (diff.z / dist);
            }
            ghost[i].position.x += ghost[i].velocity.x * dt + 0.5f * acc.x * dt * dt;
            ghost[i].position.y += ghost[i].velocity.y * dt + 0.5f * acc.y * dt * dt;
            ghost[i].position.z += ghost[i].velocity.z * dt + 0.5f * acc.z * dt * dt;
            ghost[i].velocity.x += acc.x * dt;
            ghost[i].velocity.y += acc.y * dt;
            ghost[i].velocity.z += acc.z * dt;
        }

        out[*outCount] = pos;
        (*outCount)++;
    }
}

void DrawPrediction(Vector3 *points, int count) {
    for (int i = 0; i < count - 1; i++) {
        float alpha = 1.0f - (float)i / count;
        Color c = (Color){180, 180, 180, (unsigned char)(alpha * 120)};
        if (i % 3 == 0)
            DrawLine3D(points[i], points[i+1], c);
    }
}

void DrawPredictionLine(Camera3D *camera, Region *simRegion){
    static Vector3 predicted[PREDICT_STEPS];
    static int predictCount = 0;
    if (probeCount <= 0) return;
    Probe *last = probes[probeCount-1];
    if (!last || last->launched) return;
    if (probeCount > 0 && !probes[probeCount-1]->launched) {
        Vector3 worldPos = GetMouseWorldPos(camera, simRegion);
        DrawLine3D(probes[probeCount-1]->position, worldPos, WHITE);
        DrawSphere(worldPos, 3.f, WHITE);
        float launchScale = 0.1f;
        Probe *last = probes[probeCount-1];
        Vector3 dragVel = {
            (last->position.x - worldPos.x) * launchScale,
            0,
            (last->position.z - worldPos.z) * launchScale
        };

        PredictTrajectory(probes[probeCount-1], dragVel, predicted, &predictCount);
        DrawPrediction(predicted, predictCount);
    }
}

bool IsMouseOverProbe(Camera *camera, Region *r, Probe *p) {
    Vector2 local = ScreenToRegion(r, GetMousePosition());
    Ray ray = GetScreenToWorldRayEx(local, *camera, r->dst.width, r->dst.height);
    RayCollision col = GetRayCollisionSphere(ray, p->position, 10.f);
    return col.hit;
}
