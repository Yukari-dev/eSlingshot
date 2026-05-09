#include "spaceTime.h"
#include "CelestialBody.h"

void DrawSpaceFabric(){
    int defaultY = 0;
    float massMultiplier = 4000.f;
    for(int x = -GRID_SIZE/2; x < GRID_SIZE/2; x++){
        for(int z = -GRID_SIZE/2; z < GRID_SIZE/2; z++){
            Vector3 p00 = (Vector3){x * GRID_SPACING, defaultY, z*GRID_SPACING};
            Vector3 p10 = (Vector3){(x+1) * GRID_SPACING, defaultY, z*GRID_SPACING};
            Vector3 p01 = (Vector3){x * GRID_SPACING, defaultY, (z+1)*GRID_SPACING};
            for(int i = 0; i < bodiesCount; i++){
                float softening = bodies[i].radius * bodies[i].radius * 2;
                float well      = bodies[i].mass * massMultiplier;

                float dx, dz, distSq, disp;

                dx = p00.x - bodies[i].position.x;
                dz = p00.z - bodies[i].position.z;
                distSq = dx*dx + dz*dz;
                disp = well / (distSq + softening);
                p00.y -= disp;

                dx = p10.x - bodies[i].position.x;
                dz = p10.z - bodies[i].position.z;
                distSq = dx*dx + dz*dz;
                disp = well / (distSq + softening);
                p10.y -= disp;

                dx = p01.x - bodies[i].position.x;
                dz = p01.z - bodies[i].position.z;
                distSq = dx*dx + dz*dz;
                disp = well / (distSq + softening);
                p01.y -= disp;
            }
            float alpha = 80;
            Color c = (Color){100, 100, 100, alpha};
            DrawLine3D(p00, p10, c);
            DrawLine3D(p00, p01, c);
        }
    }
}

