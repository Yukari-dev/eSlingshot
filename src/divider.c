#include "divider.h"

void DrawVerticalDivider(Rectangle r, int yOffset, int length, float thickness, Color c){
    DrawLineEx((Vector2){r.width/2.f, r.y+yOffset}, (Vector2){r.width/2.f, r.y+yOffset+length}, thickness, c);
}

void DrawHorizontalDivider(Rectangle r, int yOffset, int padding, int length, float thickness, Color c){
    DrawLineEx((Vector2){r.x+padding, r.y+yOffset}, (Vector2){r.x+r.width-padding, r.y+yOffset}, thickness, c);
}
