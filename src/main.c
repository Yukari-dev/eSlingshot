#include <stdio.h>
#include <raylib.h>
#include "region.h"
#include "rlgl.h"
#include "CelestialBody.h"

void Input(Region simRegion, Camera *camera){
    if (IsMouseInRegion(&simRegion)) {
        if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))  HideCursor();
        else ShowCursor(); 

        bool camActive = IsMouseButtonDown(MOUSE_BUTTON_RIGHT);

        
        if(camActive){
            float speed = IsKeyDown(KEY_LEFT_SHIFT) ? 2.f : 0.1f;
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

    CelestialBody sun = CreateBody("SUN", (Vector3){0, 0, 0}, 33000, 150);
    CelestialBody earth = CreateBody("EARTH", (Vector3){1000, 0, 0}, 1, 15);
    MakeOrbit(&bodies[1], &bodies[0]);

    while(!WindowShouldClose()){
        Input(simRegion, &camera);
        UpdateBodies(GetFrameTime());
        BeginRegion(&simRegion);
            ClearBackground(BLACK);
            BeginMode3D(camera);
                DrawBodies();
            EndMode3D();
        EndRegion();

        BeginDrawing();
            ClearBackground(BLACK);
            DrawRegion(&simRegion);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
