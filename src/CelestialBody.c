#include "CelestialBody.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


CelestialBody bodies[MAX_BODIES] = {0};
int bodiesCount = 0;

CelestialBody CreateBody(char *name, Vector3 position, float mass, float radius, float angularVelocity, BodyType bodyType) {
    CelestialBody b;
    b.name       = strdup(name);
    b.velocity   = (Vector3){0, 0, 0};
    b.position   = position;
    b.angularVelocity = angularVelocity;
    b.mass       = mass;
    b.radius     = radius;
    b.trace      = NULL;
    b.traceIndex = 0;
    b.traceCount = 0;
    if (bodiesCount < MAX_BODIES) {
        bodies[bodiesCount] = b;
        bodies[bodiesCount].trace = malloc(sizeof(Vector3) * TRACE_LENGTH);
        bodiesCount++;
    }
    b.bodyType = bodyType;
    return b;
}

void MakeOrbit(int bIndex, int hostIndex) {
    CelestialBody *b    = &bodies[bIndex];
    CelestialBody *host = &bodies[hostIndex];

    Vector3 diff = {
        host->position.x - b->position.x,
        host->position.y - b->position.y,
        host->position.z - b->position.z
    };

    float distSq = diff.x*diff.x + diff.y*diff.y + diff.z*diff.z;
    float dist   = sqrtf(distSq);
    float speed  = sqrtf((G * host->mass) / dist);

    Vector3 perp = { -diff.z / dist, 0, diff.x / dist };

    b->velocity.x = host->velocity.x + perp.x * speed;
    b->velocity.y = host->velocity.y + perp.y * speed;
    b->velocity.z = host->velocity.z + perp.z * speed;
}

void DrawBodies() {
    for(int i = 0; i < bodiesCount; i++){
        CelestialBody *body = &bodies[i];
        rlPushMatrix();
            rlTranslatef(body->position.x, body->position.y, body->position.z);
            rlScalef(body->radius, body->radius, body->radius);
            rlRotatef(body->angle, 0, 1, 0);

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
        bodies[i].angle += bodies[i].angularVelocity;

        bodies[i].trace[bodies[i].traceIndex] = bodies[i].position;
        bodies[i].traceIndex = (bodies[i].traceIndex + 1) % TRACE_LENGTH;
        if(bodies[i].traceCount < TRACE_LENGTH) bodies[i].traceCount++;
    }
}

void InitializeSolarSystem(){
    CreateBody("SUN",     (Vector3){0,    0, 0}, 33000, 500,  0   , BODY_STAR);
    CreateBody("MERCURY", (Vector3){500,  500, 0}, 2.f,     8,  0.7f, BODY_TERRESTRIAL);
    CreateBody("VENUS",   (Vector3){1000,  0, 0}, 3.f,    12, 0.6f, BODY_TERRESTRIAL);
    CreateBody("EARTH",   (Vector3){1500, 0, 0}, 5,      15,  0.4f, BODY_TERRESTRIAL);
    CreateBody("MARS",    (Vector3){2000, 0, 0}, 3.f,    14,  0.45f,BODY_TERRESTRIAL);
    CreateBody("JUPITER", (Vector3){0, 0, 2500}, 100,     35, 0.5f, BODY_GAS_GIANT);
    CreateBody("SATURN",  (Vector3){0, 0, 3000}, 27,     30,  0.6f, BODY_GAS_GIANT);
    CreateBody("URANUS",  (Vector3){4000, 0, 0}, 25,     25,  0.1f, BODY_ICE_GIANT);
    CreateBody("NEPTUNE", (Vector3){0, 0, 5000}, 28,     27,  0.2f, BODY_ICE_GIANT);

    for (int i = 1; i < bodiesCount; i++)
        MakeOrbit(i, 0);
}


void DrawBodyHovered(CelestialBody *b){
    Vector3 pos = b->position;
    float radius = b->radius*0.5;
    float length = radius*0.5;

    // TOP-RIGHT-BACK
    Vector3 trb00 = (Vector3){pos.x+radius,        pos.y+radius,        pos.z+radius};
    Vector3 trb01 = (Vector3){pos.x+radius-length, pos.y+radius,        pos.z+radius};
    Vector3 trb10 = (Vector3){pos.x+radius,        pos.y+radius,        pos.z+radius-length};
    Vector3 trb11 = (Vector3){pos.x+radius,        pos.y+radius-length, pos.z+radius};
    DrawLine3D(trb00, trb01, WHITE);
    DrawLine3D(trb00, trb10, WHITE);
    DrawLine3D(trb00, trb11, WHITE);

    // TOP-LEFT-BACK
    Vector3 tlb00 = (Vector3){pos.x+radius,        pos.y+radius,        pos.z-radius};
    Vector3 tlb01 = (Vector3){pos.x+radius-length, pos.y+radius,        pos.z-radius};
    Vector3 tlb10 = (Vector3){pos.x+radius,        pos.y+radius,        pos.z-radius+length};
    Vector3 tlb11 = (Vector3){pos.x+radius,        pos.y+radius-length, pos.z-radius};
    DrawLine3D(tlb00, tlb01, WHITE);
    DrawLine3D(tlb00, tlb10, WHITE);
    DrawLine3D(tlb00, tlb11, WHITE);


    // TOP-LEFT-FRONT
    Vector3 tlf00 = (Vector3){pos.x-radius,        pos.y+radius,        pos.z-radius};
    Vector3 tlf01 = (Vector3){pos.x-radius+length, pos.y+radius,        pos.z-radius};
    Vector3 tlf10 = (Vector3){pos.x-radius,        pos.y+radius,        pos.z-radius+length};
    Vector3 tlf11 = (Vector3){pos.x-radius,        pos.y+radius-length, pos.z-radius};
    DrawLine3D(tlf00, tlf01, WHITE);
    DrawLine3D(tlf00, tlf10, WHITE);
    DrawLine3D(tlf00, tlf11, WHITE);
    
    //TOP-RIGHT-FRONT
    Vector3 trf00 = (Vector3){pos.x-radius,        pos.y+radius,        pos.z+radius};
    Vector3 trf01 = (Vector3){pos.x-radius+length, pos.y+radius,        pos.z+radius};
    Vector3 trf10 = (Vector3){pos.x-radius,        pos.y+radius,        pos.z+radius-length};
    Vector3 trf11 = (Vector3){pos.x-radius,        pos.y+radius-length, pos.z+radius};
    DrawLine3D(trf00, trf01, WHITE);
    DrawLine3D(trf00, trf10, WHITE);
    DrawLine3D(trf00, trf11, WHITE);
    
    //BOTTOM-RIGHT-FRONT
    Vector3 brb00 = (Vector3){pos.x-radius,        pos.y-radius,        pos.z+radius};
    Vector3 brb01 = (Vector3){pos.x-radius+length, pos.y-radius,        pos.z+radius};
    Vector3 brb10 = (Vector3){pos.x-radius,        pos.y-radius,        pos.z+radius-length};
    Vector3 brb11 = (Vector3){pos.x-radius,        pos.y-radius+length, pos.z+radius};
    DrawLine3D(brb00, brb01, WHITE);
    DrawLine3D(brb00, brb10, WHITE);
    DrawLine3D(brb00, brb11, WHITE);

    //BOTTOM-RIGHT-BACK
    Vector3 blb00 = (Vector3){pos.x+radius,        pos.y-radius,        pos.z+radius};
    Vector3 blb01 = (Vector3){pos.x+radius-length, pos.y-radius,        pos.z+radius};
    Vector3 blb10 = (Vector3){pos.x+radius,        pos.y-radius,        pos.z+radius-length};
    Vector3 blb11 = (Vector3){pos.x+radius,        pos.y-radius+length, pos.z+radius};
    DrawLine3D(blb00, blb01, WHITE);
    DrawLine3D(blb00, blb10, WHITE);
    DrawLine3D(blb00, blb11, WHITE);

    //BOTTOM-LEFT-BACK
    Vector3 brf00 = (Vector3){pos.x+radius,        pos.y-radius,        pos.z-radius};
    Vector3 brf01 = (Vector3){pos.x+radius-length, pos.y-radius,        pos.z-radius};
    Vector3 brf10 = (Vector3){pos.x+radius,        pos.y-radius,        pos.z-radius+length};
    Vector3 brf11 = (Vector3){pos.x+radius,        pos.y-radius+length, pos.z-radius};
    DrawLine3D(brf00, brf01, WHITE);
    DrawLine3D(brf00, brf10, WHITE);
    DrawLine3D(brf00, brf11, WHITE);

    //BOTTOM-LEFT-FRONT
    Vector3 blf00 = (Vector3){pos.x-radius,        pos.y-radius,        pos.z-radius};
    Vector3 blf01 = (Vector3){pos.x-radius+length, pos.y-radius,        pos.z-radius};
    Vector3 blf10 = (Vector3){pos.x-radius,        pos.y-radius,        pos.z-radius+length};
    Vector3 blf11 = (Vector3){pos.x-radius,        pos.y-radius+length, pos.z-radius};
    DrawLine3D(blf00, blf01, WHITE);
    DrawLine3D(blf00, blf10, WHITE);
    DrawLine3D(blf00, blf11, WHITE);
}

bool IsMouseOverBody(Camera3D *camera, Region *r, CelestialBody *body) {
    Vector2 local = ScreenToRegion(r, GetMousePosition());
    Ray ray = GetScreenToWorldRayEx(local, *camera, r->dst.width, r->dst.height);
    RayCollision col = GetRayCollisionSphere(ray, body->position, body->radius * 0.5f);
    return col.hit;
}

bool IsMouseOverAnyBody(Camera3D *camera, Region *r){
    for(int i = 0; i < bodiesCount; i++){
        CelestialBody *body = &bodies[i];
        Vector2 local = ScreenToRegion(r, GetMousePosition());
        Ray ray = GetScreenToWorldRayEx(local, *camera, r->dst.width, r->dst.height);
        RayCollision col = GetRayCollisionSphere(ray, body->position, body->radius * 0.5f);
        if(col.hit)
            return true;
    }
    return false;
}

void DrawHoveredBodies(Camera3D *camera, Region *simRegion){
    for(int i = 0; i < bodiesCount; i++){
        if(IsMouseOverBody(camera, simRegion, &bodies[i])){
            DrawBodyHovered(&bodies[i]);
        }
    }
}

char *GetBodyTypeName(CelestialBody *b){
    switch(b->bodyType){
        case BODY_STAR: return "STAR";
        case BODY_GAS_GIANT: return "GAS";
        case BODY_TERRESTRIAL: return "TERA";
        case BODY_ICE_GIANT: return "ICE";
        case BODY_DWARF: return "DWARF";
    }
}
