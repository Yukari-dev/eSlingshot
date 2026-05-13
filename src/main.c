#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>
#include "CelestialBody.h"
#include "spaceTime.h"
#include "probes.h"
#include "input.h"
#include "widget.h"
#include "bodyInfoPanel.h"
#include "font.h"


int main() {
    SetTraceLogLevel(LOG_ERROR);
    InitWindow(0, 0, "eSlingShot v0.1");

    LoadGlobalFont("fonts/ShareTechMono.ttf");
    SetTargetFPS(60);
    int w = GetScreenWidth(), h = GetScreenHeight();

    Region simRegion = CreateRegion(0, 0, 700, 500, true, true);
    CenterRegion(&simRegion);
    MoveRegion(&simRegion, -1, 50);
    Region leftRegion = CreateRegion(10, 50, 300, 500, false, true);

    BodyInfoPanel bodyInfo = CreateBodyInfoPanel(10, 50, 300, 300);

    Camera3D camera = {0};
    camera.position   = (Vector3){1000.f, 1000.f, 0.f};
    camera.target     = (Vector3){0.f, 0.f, 0.f};
    camera.up         = (Vector3){0.f, 1.f, 0.f};
    camera.fovy       = 45.f;
    camera.projection = CAMERA_PERSPECTIVE;

    InitializeSolarSystem();
    int sw = GetScreenWidth();
    int sh = GetScreenHeight();
    Rectangle bottomBar = { 0, sh - 130, sw, 130 };

    double timeMultiplier = 2;

    while (!WindowShouldClose()) {
        Input(&simRegion, &camera);
        float dt = GetFrameTime()*timeMultiplier;
        UpdateBodies(dt);
        UpdateProbes(dt);
        UpdateBodyInfoPanel(&bodyInfo);

        BeginRegion(&simRegion);
            ClearBackground(BLACK);
            BeginMode3D(camera);
                rlSetClipPlanes(0.01, 20000.0);
                DrawBodies();
                DrawTraces();
                DrawSpaceFabric();
                DrawProbes();
                DrawProbesTraces();
                DrawPredictionLine(&camera, &simRegion);
                bool clickedAny = false;

                for (int i = 0; i < bodiesCount; i++) {
                    bool isHovering = IsMouseOverBody(&camera, &simRegion, &bodies[i]);
                    if (bodyInfo.selected == &bodies[i])
                        DrawBodyHovered(&bodies[i]);
                    if (isHovering) {
                        DrawBodyHovered(&bodies[i]);
                        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                            SelectBody(&bodyInfo, &bodies[i]);
                            clickedAny = true;
                        }
                    }
                }

                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && !clickedAny)
                    SelectBody(&bodyInfo, NULL);
            EndMode3D();
        EndRegion();
        

        BeginDrawing();
            ClearBackground(BLACK);
            DrawRegion(&simRegion);
            DrawRegion(&leftRegion);
            DrawBodyInfoPanel(&bodyInfo);
            WidgetSpeedGraph(bottomBar);

        EndDrawing();
    }

    UnloadGlobalFont();

    FreeProbes();
    CloseWindow();
    return 0;
}
