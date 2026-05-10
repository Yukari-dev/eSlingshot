#include "input.h"
#include "probes.h"
#include "CelestialBody.h"

void Input(Region *simRegion, Camera *camera) {
    bool inRegion = IsMouseInRegion(simRegion);
    if(!inRegion) return;

    if (inRegion) {
        if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) HideCursor();
        else ShowCursor();

        if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
            float speed = IsKeyDown(KEY_LEFT_SHIFT) ? 10.f : 1.f;
            UpdateCameraPro(camera,
                (Vector3){
                    (IsKeyDown(KEY_W) - IsKeyDown(KEY_S)) * speed,
                    (IsKeyDown(KEY_D) - IsKeyDown(KEY_A)) * speed,
                    (IsKeyDown(KEY_E) - IsKeyDown(KEY_Q)) * speed
                },
                (Vector3){ GetMouseDelta().x * 0.1f, GetMouseDelta().y * 0.1f, 0.0f },
                0.0f
            );
            SetMousePosition(GetScreenWidth()/2, GetScreenHeight()/2);
        } else {
            Vector3 worldPos = GetMouseWorldPos(camera,simRegion);

            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && inRegion && !IsMouseOverAnyBody(camera, simRegion))
                AddProbe(worldPos);
            
            if (probeCount > 0) {
                Probe *last = probes[probeCount - 1];
                if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && !last->launched && inRegion) {
                    float launchScale = 0.1f;
                    last->velocity = (Vector3){
                        (last->position.x - worldPos.x) * launchScale,
                        0,
                        (last->position.z - worldPos.z) * launchScale
                    };
                    last->launched = true;
                }
            }
        }
    } else {
        EnableCursor();
    }
}

Vector3 GetMouseWorldPos(Camera3D *camera, Region *r){
    Vector2 mouse = GetMousePosition();

    Vector2 local = ScreenToRegion(r, mouse);

    Ray ray = GetScreenToWorldRayEx(local, *camera, r->dst.width, r->dst.height);

    if (fabsf(ray.direction.y) < 0.0001f) return (Vector3){0, 0, 0};

    float t = -ray.position.y / ray.direction.y;
    return (Vector3){
        ray.position.x + t * ray.direction.x,
        0,
        ray.position.z + t * ray.direction.z
    };
}
