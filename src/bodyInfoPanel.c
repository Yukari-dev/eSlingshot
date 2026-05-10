#include "bodyInfoPanel.h"
#include "border.h"
#include "widget.h"
#include "font.h"
#include <stdio.h>
#define BODY_INFO_FONT_SIZE 32

static float GravStrength(CelestialBody *b) {
    float r = b->radius * 0.5f;
    return (G * b->mass) / (r * r);
}
 
static float CurvatureIndex(CelestialBody *b) {
    return b->mass / (b->radius + 1.f);
}

BodyInfoPanel CreateBodyInfoPanel(float x, float y, float w, float h){
    BodyInfoPanel bip = {0};
    bip.panel = CreatePanel(x, y, w, h, NULL);
    float mvW = w - 20;
    float mvH = h * 0.38f;
    bip.mv = CreateMiniViewport(x+10, y+10, mvW, mvH);
    bip.selected = NULL;
    return bip;
}

void UpdateBodyInfoPanel(BodyInfoPanel *bip){
    if(!bip->selected) return;
    UpdateMiniViewport(&bip->mv);
}

void SelectBody(BodyInfoPanel *bip, CelestialBody *body){
    bip->selected = body;
}

void DrawBodyInfoPanel(BodyInfoPanel *b){
    if(!b->panel.active) return;

    // DrawRectangle(b->panel.dst.x, b->panel.dst.y,
    //               b->panel.dst.width, b->panel.dst.height,
    //               (Color){5, 5, 5, 240});
    // DrawFourCornerBorder(b->panel.dst, 10, WHITE);

    if (!b->selected) {
        int tx = b->panel.dst.x + b->panel.dst.width  / 2 - MeasureTextEx(globalFont, "NO SELECTION", BODY_INFO_FONT_SIZE, 1).x / 2;
        int ty = b->panel.dst.y + b->panel.dst.height / 2 - BODY_INFO_FONT_SIZE / 2;
        DrawTextEx(globalFont, "NO SELECTION", (Vector2){tx, ty}, BODY_INFO_FONT_SIZE, 1, (Color){50, 50, 50, 255});
        return;
    }

    DrawMiniViewport(&b->mv, b->selected);

    int mvBottom = b->mv.r.dst.y+b->mv.r.dst.height;
    int cx = b->panel.dst.x + PANEL_PADDING;
    int cy = mvBottom+12;
    int pw = b->panel.dst.width - PANEL_PADDING*2;

    int nameFontSize = 32;
    char nameBuf[64];
    snprintf(nameBuf, sizeof(nameBuf), "[%s]", GetBodyTypeName(b->selected));
    int nameW = MeasureTextEx(globalFont, b->selected->name, nameFontSize, 1).x;
    DrawTextEx(globalFont, nameBuf, (Vector2){b->panel.dst.x+PANEL_PADDING, cy}, 28, 1, WHITE);
    DrawTextEx(globalFont, b->selected->name, (Vector2){b->panel.dst.x + b->panel.dst.width/2 - nameW/2, cy}, nameFontSize, 1, WHITE);
    cy+=30;

    DrawLine(cx, cy, cx + pw, cy, (Color){40, 40, 40, 255});
    cy += 10;
    
    WidgetMass(&b->panel, b->selected->mass, bodies[0].mass);
    WidgetRadius(&b->panel, b->selected->radius, bodies[0].radius);

}

