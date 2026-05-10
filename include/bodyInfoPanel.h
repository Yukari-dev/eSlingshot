#ifndef BODYINFOPANEL_H
#define BODYINFOPANEL_H
#include <raylib.h>
#include "panel.h"
#include "miniViewport.h"
#include "CelestialBody.h"

typedef struct {
    Panel panel;
    MiniViewport mv;
    CelestialBody *selected;
} BodyInfoPanel;

BodyInfoPanel CreateBodyInfoPanel(float x, float y, float w, float h);
void UpdateBodyInfoPanel(BodyInfoPanel *bip);
void DrawBodyInfoPanel(BodyInfoPanel *bip);
void SelectBody(BodyInfoPanel *bip, CelestialBody *body);

#endif
