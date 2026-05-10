#include "widget.h"
#include "font.h"
#include <stdio.h>
#include <math.h>


static int PX(Panel *p) { return p->dst.x + PANEL_PADDING; }
static int PY(Panel *p) { return p->dst.y + p->cursorY;    }
static int PW(Panel *p) { return p->dst.width - PANEL_PADDING * 2; }


void WidgetLabel(Panel *p, const char *text){
    if(!p || !p->active) return;
    DrawText(text, PX(p), PY(p), PANEL_FONT_SIZE, (Color){180, 180, 180, 255});
    p->cursorY += PANEL_LINE_HEIGHT;
}

void WidgetValue(Panel *p, const char *label, double value){
    if(!p || !p->active) return;

    char buff[64];
    snprintf(buff, sizeof(buff), "%s: %f", label, value);
    
    DrawText(buff, PX(p), PY(p), PANEL_FONT_SIZE, (Color){180, 180, 180, 255});

    p->cursorY += PANEL_LINE_HEIGHT;
}

void WidgetDivider(Panel *p){
    if(!p || !p->active) return;
    p->cursorY += PANEL_LINE_HEIGHT;
    
    DrawLine(PX(p), PY(p), PX(p)+PW(p), PY(p), WHITE);

    p->cursorY += PANEL_LINE_HEIGHT;
}

bool WidgetButton(Panel *p, const char *label){
    if(!p || !p->active) return false;

    Rectangle btn = (Rectangle){PX(p), PY(p), PW(p), PANEL_LINE_HEIGHT+10};
    bool hovered  = CheckCollisionPointRec(GetMousePosition(), btn);
    bool clicked  = hovered && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);

    Color bg      = hovered ? WHITE : BLACK;
    Color border  = hovered ? BLACK : WHITE;
    Color text    = hovered ? BLACK : WHITE;

    int tx = btn.x + (btn.width  - MeasureText(label, PANEL_FONT_SIZE)) / 2;
    int ty = btn.y + (btn.height - PANEL_FONT_SIZE) / 2;
    DrawText(label, tx, ty, PANEL_FONT_SIZE, text);
    DrawRectangleRec(btn, bg);
    DrawRectangleLinesEx(btn, 1, border);
 
    p->cursorY += PANEL_LINE_HEIGHT + 14;
    return clicked;
}


void WidgetProgressBar(Panel *p, const char *label, float value) {
    if (!p || !p->active) return;
 
    DrawText(label, PX(p), PY(p), PANEL_FONT_SIZE, (Color){100, 100, 100, 255});
    p->cursorY += PANEL_LINE_HEIGHT+4;
 
    Rectangle bg  = { PX(p), PY(p), PW(p),            4 };
    Rectangle bar = { PX(p), PY(p), PW(p) * value,     4 };
 
    DrawRectangleRec(bg,  (Color){30, 30, 30, 255});
    DrawRectangleRec(bar, WHITE);
 
    p->cursorY += 10;
}

#define MASS_GRID_COLS 10
#define MASS_GRID_ROWS 5
#define MASS_CELL_SIZE 8
#define MASS_GRID_GAP  2

void WidgetMass(Panel *p, float mass, float maxMass){
    if (!p || !p->active) return;
    p->cursorY = 170;

    Rectangle r = p->dst;
    int cx = r.x + PANEL_PADDING;
    int cy = r.y + p->cursorY;
    int pw = r.width - PANEL_PADDING*2;
    

    char val[32];
    snprintf(val, sizeof(val), "MASS:%.0fKG", mass);
    DrawTextEx(globalFont, val, (Vector2){cx, cy}, 16, 1, WHITE);

    p->cursorY += PANEL_LINE_HEIGHT;
    cy = r.y+p->cursorY;

    int totalCells = MASS_GRID_COLS * MASS_GRID_ROWS;
    float ratio = logf(mass + 1.f) / logf(maxMass + 1.f);
    int filled = (int)(ratio*totalCells);
    int gridW = MASS_GRID_COLS * (MASS_CELL_SIZE + MASS_GRID_GAP) - MASS_GRID_GAP;

    int startX = cx + (pw - gridW)/2;

    for(int row = 0; row < MASS_GRID_ROWS; row++){
        for(int col = 0; col < MASS_GRID_COLS; col++){
            int idx = row * MASS_GRID_COLS + col;
            int x = cx + col * (MASS_CELL_SIZE + MASS_GRID_GAP);
            int y = cy + row * (MASS_CELL_SIZE + MASS_GRID_GAP);

            if(idx < filled){
                float brightness = 0.4f + 0.6f * ((float)idx / totalCells);
                unsigned char b  = (unsigned char)(brightness * 255);
                DrawRectangle(x, y, MASS_CELL_SIZE, MASS_CELL_SIZE, (Color){b, b, b, 255});
            } else{
                DrawRectangleLines(x, y, MASS_CELL_SIZE, MASS_CELL_SIZE, (Color){30, 30, 30, 255});
            }
        }
    }

    p->cursorY += MASS_GRID_ROWS * (MASS_CELL_SIZE + MASS_GRID_GAP) + 8;
}

void WidgetRadius(Panel *p, float radius, float sunRadius){
    if (!p || !p->active) return;


    Rectangle r = p->dst;
    int cx = r.x + PANEL_PADDING;
    int cy = r.y + p->cursorY;
    int pw = r.width - PANEL_PADDING*2;

    char val[32];
    snprintf(val, sizeof(val), "RADIUS:%.0fKM", radius);
    DrawTextEx(globalFont, val, (Vector2){cx, cy}, 16, 1, WHITE);

    p->cursorY += PANEL_LINE_HEIGHT;
    cy = r.y+p->cursorY;

    int maxR = 20;
    int sunR = maxR;
    int bodyR = (int)fmaxf((logf(radius + 1.f) / logf(sunRadius + 1.f)) * maxR, 2.f);


    int centerY = cy + maxR + 4;
    int leftX = cx + maxR;
    int rightX = cx + pw/4 + maxR + 4;
    
    DrawCircleLines(leftX, centerY, sunR, (Color){60, 60, 60, 255});
    DrawTextEx(globalFont, "SUN", (Vector2){leftX-MeasureTextEx(globalFont, "SUN", 12, 1).x/2, centerY+maxR+4}, 12, 1, (Color){60, 60, 60, 255});

    if(radius != sunRadius){
        DrawCircleLines(rightX, centerY, bodyR, (Color){60, 60, 60, 255});
        DrawTextEx(globalFont, p->title ? p->title : "BODY",
            (Vector2){rightX - MeasureTextEx(globalFont, p->title ? p->title : "BODY", 9, 1).x/2,
            centerY + maxR + 3}, 12, 1, (Color){150, 150, 150, 255});
    }


    p->cursorY += maxR * 2 + 20;
}
