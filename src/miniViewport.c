#include "miniViewport.h"
#define GRID_SIZE    100
#define GRID_SPACING 50.f

MiniViewport CreateMiniViewport(float x, float y, float w, float h){
    MiniViewport mv = {0};
    mv.r = CreateRegion(x, y, w, h, true, false);
    mv.camera.position = (Vector3){0, 0.f, 500.f};
    mv.camera.target = (Vector3){0, -0.3f, 0};
    mv.camera.up = (Vector3){0, 1, 0};
    mv.camera.fovy = 40.f;
    mv.camera.projection = CAMERA_PERSPECTIVE;
    mv.angle = 0.f;
    return mv;
}

static void DrawMiniBody(CelestialBody *body, float angle){
    rlPushMatrix();
        rlScalef(body->radius, body->radius, body->radius);
        rlRotatef(angle, 0, 1, 0);

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

static void DrawMiniFabric(CelestialBody *body){
    int defaultY = 0;
    float massMultiplier = 4000.f;
    for(int x = -GRID_SIZE/2; x < GRID_SIZE/2; x++){
        for(int z = -GRID_SIZE/2; z < GRID_SIZE/2; z++){
            Vector3 p00 = (Vector3){x * GRID_SPACING, defaultY, z*GRID_SPACING};
            Vector3 p10 = (Vector3){(x+1) * GRID_SPACING, defaultY, z*GRID_SPACING};
            Vector3 p01 = (Vector3){x * GRID_SPACING, defaultY, (z+1)*GRID_SPACING};
            float softening = body->radius * body->radius;
            float well      = body->mass * massMultiplier;

            float dx, dz, distSq, disp;

            dx = p00.x;
            dz = p00.z;
            distSq = dx*dx + dz*dz;
            disp = well / (distSq + softening);
            p00.y -= disp;

            dx = p10.x;
            dz = p10.z;
            distSq = dx*dx + dz*dz;
            disp = well / (distSq + softening);
            p10.y -= disp;

            dx = p01.x;
            dz = p01.z;
            distSq = dx*dx + dz*dz;
            disp = well / (distSq + softening);
            p01.y -= disp;
            float alpha = 80;
            Color c = (Color){100, 100, 100, alpha};
            DrawLine3D(p00, p10, c);
            DrawLine3D(p00, p01, c);
        }
    }
}

void UpdateMiniViewport(MiniViewport *mv){
    mv->angle += 0.3f;
    if(mv->angle > 360.f) mv->angle -= 360.f;
}

void DrawMiniViewport(MiniViewport *mv, CelestialBody *body) {
    if (!mv || !mv->r.active) return;

    float softening   = body->radius * body->radius * 2;
    float well        = body->mass * 4000.f;
    float maxDisp     = well / (0 + softening);
    float clampedDisp = fminf(maxDisp, 1000.f);

    float zoom = body->radius * 5;

    mv->camera.position = (Vector3){ 0, clampedDisp, zoom + body->radius };
    mv->camera.target   = (Vector3){ 0, -clampedDisp * 0.3f, 0 };

    BeginRegion(&mv->r);
        ClearBackground((Color){5, 5, 5, 255});
        BeginMode3D(mv->camera);
            DrawMiniBody(body, mv->angle);
            DrawMiniFabric(body);
        EndMode3D();
    EndRegion();

    DrawRegion(&mv->r);
}

