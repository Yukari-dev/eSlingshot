#ifndef REGION_H
#define REGION_H
#include <raylib.h>
#include <stdbool.h>

typedef struct{
    RenderTexture2D texture;
    Rectangle dst;
    float borderThickness;
    Color borderColor;
    bool active;
    bool hasBorder;
} Region;

Region CreateRegion(float x, float y, float w, float h);
void DestroyRegion(Region *r);

void BeginRegion(Region *r);
void EndRegion(void);

void DrawRegion(Region *r);
void DrawRegionBorder(Region *r);

void CenterRegion(Region *r);
void MoveRegion(Region *r, float x, float y);
bool IsMouseInRegion(Region *r);
Vector2 ScreenToRegion(Region *r, Vector2 pos);
Vector2 RegionToScreen(Region *r, Vector2 pos);

#endif
