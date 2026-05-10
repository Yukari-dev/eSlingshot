#include "region.h"
#include "border.h"

Region CreateRegion(float x, float y, float w, float h, bool isTexture, bool hasBorder){
    Region r = {0};
    if(isTexture)
        r.texture = LoadRenderTexture(w, h);
    // r.texture = isTexture ? LoadRenderTexture(w, h) : NULL;
    r.dst = (Rectangle){x, y, w, h};
    r.borderThickness = 1.f;
    r.borderColor = WHITE;
    r.hasBorder = hasBorder;
    r.active = true;
    return r;
}

void DestroyRegion(Region *r){
    if(!r || r->texture.texture.id == 0) return;
    UnloadRenderTexture(r->texture);
    r->active = false;
}

void BeginRegion(Region *r){
    if(!r || !r->active) return;
    BeginTextureMode(r->texture);
}

void EndRegion(void){
    EndTextureMode();
}

void DrawRegion(Region *r){
    if(!r || !r->active) return;

    Rectangle src = (Rectangle){
        0, 0,
        (float)r->texture.texture.width,
        -(float)r->texture.texture.height
    };

    if(r->texture.texture.id != 0)
        DrawTexturePro(r->texture.texture, src, r->dst, (Vector2){0, 0}, 0.0f, WHITE);
    if(r->hasBorder) DrawRegionBorder(r);
}

void DrawRegionBorder(Region *r){
    if(!r || !r->active) return;
    int length = 15;
    DrawTwoCornerBorder(r->dst, length, WHITE);
    float padding = length+15;
    Rectangle b = r->dst;
    Color color = (Color){25, 25, 25, 225};

    DrawLine(b.x+padding, b.y, b.x+b.width, b.y, color);
    DrawLine(b.x+b.width, b.y, b.x+b.width, b.y+b.height-padding, color);
    DrawLine(b.x+b.width-padding, b.y+b.height, b.x, b.y+b.height, color);
    DrawLine(b.x, b.y+b.height, b.x, b.y+padding, color);
}

void CenterRegion(Region *r){
    if(!r || !r->active) return;
    r->dst.x = (GetScreenWidth() - r->dst.width) / 2.f;
    r->dst.y = (GetScreenHeight() - r->dst.height) / 2.f;
}

void MoveRegion(Region *r, float x, float y){
    if(!r || !r->active) return;
    r->dst.x = x >= 0 ? x : r->dst.x;
    r->dst.y = y;
}

bool IsMouseInRegion(Region *r){
    if(!r || !r->active) return false;
    return CheckCollisionPointRec(GetMousePosition(), r->dst);
}

Vector2 ScreenToRegion(Region *r, Vector2 pos){
    if(!r) return (Vector2){0, 0};
    return (Vector2){
        pos.x - r->dst.x,
        pos.y - r->dst.y
    };
}

Vector2 RegionToScreen(Region *r, Vector2 pos){
    if(!r) return (Vector2){0, 0};
    return (Vector2){
        pos.x + r->dst.x,
        pos.y + r->dst.y
    };
}

