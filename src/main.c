#include <stdio.h>
#include <raylib.h>
#include "region.h"
#include "rlgl.h"
#include "CelestialBody.h"
#include "spaceTime.h"

void Input(Region simRegion, Camera *camera){
    if (IsMouseInRegion(&simRegion)) {
        if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))  HideCursor();
        else ShowCursor(); 
        bool camActive = IsMouseButtonDown(MOUSE_BUTTON_RIGHT);
        if(camActive){
            float speed = IsKeyDown(KEY_LEFT_SHIFT) ? 10.f : 0.1f;
            UpdateCameraPro(camera,
                (Vector3){
                    (IsKeyDown(KEY_W) - IsKeyDown(KEY_S)) * speed,   
                    (IsKeyDown(KEY_D) - IsKeyDown(KEY_A)) * speed,   
                    (IsKeyDown(KEY_E) - IsKeyDown(KEY_Q)) * speed
                },
                (Vector3){
                    GetMouseDelta().x * 0.1f,   
                    GetMouseDelta().y * 0.1f,   
                    0.0f
                },
                0.0f
            );
        }
        
    } else {
        EnableCursor();
    }
}

int main(){
    SetTraceLogLevel(LOG_ERROR);
    InitWindow(0, 0, "eSlingShot v0.1");
    SetTargetFPS(60);

    Region simRegion = CreateRegion(0, 0, 700, 500);
    CenterRegion(&simRegion);
    MoveRegion(&simRegion, -1, 50);

    Camera3D camera = {0};
    camera.position = (Vector3){1000.f, 1000.f, 0.f};
    camera.target = (Vector3){0.f, 0.f, 0.f};
    camera.up = (Vector3){0.f, 1, 0.f};
    camera.fovy = 45.f;
    camera.projection = CAMERA_PERSPECTIVE;


    CelestialBody sun         = CreateBody("SUN",     (Vector3){0,    0, 0}, 10000,  300);
    CelestialBody mercury     = CreateBody("MERCURY", (Vector3){250,  0, 0}, 2.f,    8);
    CelestialBody venus       = CreateBody("VENUS",   (Vector3){500,  0, 0}, 3.f,   12);
    CelestialBody earth       = CreateBody("EARTH",   (Vector3){1000, 0, 0}, 4,     15);
    CelestialBody mars        = CreateBody("MARS",    (Vector3){1500, 0, 0}, 3.f,   14);
    CelestialBody jupiter     = CreateBody("JUPITER", (Vector3){2000, 0, 0}, 30,    35);
    CelestialBody saturn      = CreateBody("SATURN",  (Vector3){3000, 0, 0}, 27,    30);
    CelestialBody uranus      = CreateBody("URANUS",  (Vector3){4000, 0, 0}, 25,    25);
    CelestialBody neptune     = CreateBody("NEPTUNE", (Vector3){5000, 0, 0}, 28,    27);
    for(int i = 1; i < sizeof(bodies) / sizeof(bodies[0]); i++){
        MakeOrbit(i, 0);
        // MakeOrbit(&bodies[2], &bodies[0]);
        // MakeOrbit(&bodies[3], &bodies[0]);
        // MakeOrbit(&bodies[4], &bodies[0]);
    }

    float timeMultiplier = 10.f;

    while(!WindowShouldClose()){
        Input(simRegion, &camera);
        float dt = GetFrameTime()*timeMultiplier;
        UpdateBodies(dt);
        BeginRegion(&simRegion);
            ClearBackground(BLACK);
            BeginMode3D(camera);
                rlSetClipPlanes(0.01, 20000.0); 
                DrawBodies();
                DrawTraces();
                DrawSpaceFabric();
            EndMode3D();
        EndRegion();

        BeginDrawing();
            ClearBackground(BLACK);
            char fpslabel[12];
            snprintf(fpslabel, sizeof(fpslabel), "FPS:%d", GetFPS());
            DrawText(fpslabel, 100, 100, 16, WHITE);
            DrawRegion(&simRegion);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
