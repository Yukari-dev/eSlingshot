#include "widget.h"
#include "font.h"
#include <stdio.h>
#include <math.h>
#include "probes.h"


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

void WidgetRadiusDistance(Panel *p, CelestialBody *body, CelestialBody *host){
    if (!p || !p->active) return;

    float radius = body->radius;
    float sunRadius = host->radius;


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
    int rightX = cx + pw - maxR + 4;
    
    DrawCircleLines(leftX, centerY, sunR, (Color){60, 60, 60, 255});
    DrawTextEx(globalFont, "SUN", (Vector2){leftX-MeasureTextEx(globalFont, "SUN", 12, 1).x/2, centerY+maxR+4}, 12, 1, (Color){60, 60, 60, 255});

    if(radius != sunRadius){
        DrawCircleLines(rightX, centerY, bodyR, (Color){60, 60, 60, 255});
        DrawTextEx(globalFont, p->title ? p->title : "BODY",
            (Vector2){rightX - MeasureTextEx(globalFont, p->title ? p->title : "BODY", 9, 1).x/2,
            centerY + maxR + 3}, 12, 1, (Color){150, 150, 150, 255});
    }

    float maxDistance = 5000.f;
    float dist = sqrtf(body->position.x * body->position.x + body->position.z * body->position.z);
    float ratio = (int)fminf(dist/maxDistance,1.f);
    int dotX = cx + (int)(ratio * pw);

    if(radius != sunRadius){
        for (int x = cx+sunR; x < cx + pw-bodyR-5; x += 6)
            DrawRectangle(x, cy + sunR+3, 3, 1, (Color){40, 40, 40, 255});


        p->cursorY += 30;
        cy = r.y+p->cursorY;
        char distVal[32];
        snprintf(distVal, sizeof(val), "%.0fKM", dist);
        DrawTextEx(globalFont, distVal, (Vector2){
            cx+pw/2-MeasureTextEx(globalFont, distVal, 16, 1).x/2, cy   
        }, 16, 1, (Color){60, 60, 60, 255});
        
        p->cursorY += sunR*2;
        cy = r.y+p->cursorY;

        int ticks = 8;
        int startX = cx;
        for (int i = 0; i <= ticks; i++) {
            int tx     = startX + (int)((float)i / ticks * pw);
            int height = (i % 4 == 0) ? 6 : 3;
            DrawLine(tx, cy, tx, cy - height, (Color){50, 50, 50, 255});
        }
        p->cursorY += maxR-10;
        return;
    }
    p->cursorY += maxR * 2 + 20;
}

void WidgetVelocity(Panel *p, Vector3 velocity){
    if (!p || !p->active) return;

    Rectangle r = p->dst;
    int cx = r.x + PANEL_PADDING;
    int cy = r.y + p->cursorY;
    int pw = r.width - PANEL_PADDING*2;

    DrawTextEx(globalFont, "VELOCITY", (Vector2){cx, cy}, 16, 1, WHITE);
    
    p->cursorY += PANEL_LINE_HEIGHT;
    cy = p->dst.y + p->cursorY;
    float components[3] = { velocity.x, velocity.y, velocity.z };
    char  labels[3]     = { 'X', 'Y', 'Z' };
    float maxSpeed = sqrtf(velocity.x*velocity.x + velocity.y*velocity.y + velocity.z*velocity.z) * 2.f;

    for(int i = 0; i < 3; i++){
        float val = components[i];
        float ratio = fmaxf(fminf(val / maxSpeed, 1.f), -1.f);
        int barW = pw-50;
        int centerX = cx+16+barW/2;
        int barY = cy + i * (PANEL_LINE_HEIGHT)+4;

        char label[2] = {labels[i], '\0'};
        DrawTextEx(globalFont, label, (Vector2){cx, barY-4}, 16,1, (Color){100, 100, 100, 255});

        DrawLine(cx+16, barY+4, cx+16+barW, barY+4, (Color){30, 30, 30, 255});
        DrawLine(centerX, barY, centerX, barY+8, (Color){30, 30, 30, 255});

        int fillW = (int)(ratio * (barW/2));
        if(fillW > 0)
            DrawRectangle(centerX, barY-2, fillW, 4, WHITE);
        else if(fillW < 0)
            DrawRectangle(centerX + fillW, barY-2, -fillW, 4, WHITE);

        char valStr[16];
        snprintf(valStr, sizeof(valStr), "%.1f", val);
        DrawTextEx(globalFont, valStr,
            (Vector2){p->dst.x + p->dst.width - PANEL_PADDING - MeasureText(valStr, PANEL_FONT_SIZE),
            barY - 4}, 16, 1, (Color){150, 150, 150, 255});
    }

    p->cursorY += 3 * PANEL_LINE_HEIGHT + 8;
}

void WidgetOrbitalClock(Panel *p, CelestialBody *body){
    if(!p || !p->active) return;

    Rectangle r = p->dst;
    int cx = r.x + PANEL_PADDING;
    int cy = r.y + p->cursorY;
    int pw = r.width - PANEL_PADDING*2;

    DrawTextEx(globalFont, "ORBITAL POSITION", (Vector2){cx, cy}, PANEL_FONT_SIZE, 1, (Color){100, 100, 100, 255});
    p->cursorY += PANEL_LINE_HEIGHT;
    cy = p->dst.y + p->cursorY;
    
    int radius = 28;
    int centerX = r.x + r.width/2;
    int centerY = cy+radius+4;

    float angle = atan2(body->position.z, body->position.x);
    if(angle < 0) angle += 2.f * PI;
    float degrees = angle * (180.f*PI);
    float percent = angle / (2.f*PI);

    DrawCircleLines(centerX, centerY, radius, (Color){30, 30, 30, 255});

    DrawCircleSector(
        (Vector2){centerX, centerY},
        radius - 4,
        -90,
        -90 + degrees,
        36,
        (Color){60, 60, 60, 255}
    );

    DrawCircleSectorLines(
        (Vector2){centerX, centerY},
        radius,
        -90,
        -90 + degrees,
        36,
        WHITE
    );

    float dotAngle = angle - PI / 2.f;
    int   dotX     = centerX + (int)(cosf(dotAngle) * radius);
    int   dotY     = centerY + (int)(sinf(dotAngle) * radius);
    DrawCircle(dotX, dotY, 3, WHITE);

    char pct[8];
    snprintf(pct, sizeof(pct), "%03d%%", (int)(percent * 100));
    DrawTextEx(globalFont, pct,
        (Vector2){centerX - MeasureText(pct, PANEL_FONT_SIZE) / 2,
        centerY - PANEL_FONT_SIZE / 2},
        PANEL_FONT_SIZE, 1, WHITE);

    DrawCircle(centerX, centerY - radius, 2, (Color){180, 180, 180, 255});

    p->cursorY += radius * 2 + 20;

}


void WidgetSpeedGraph(Rectangle dst) {
    int pad   = 30;
    int gx    = dst.x + pad;
    int gy    = dst.y + 10;
    int gw    = dst.width - pad - 10;
    int gh    = dst.height - 24;
 
    DrawRectangle(dst.x, dst.y, dst.width, dst.height, (Color){5, 5, 5, 240});
    DrawRectangleLines(gx, gy, gw, gh, (Color){30, 30, 30, 255});
 
    DrawTextEx(globalFont, "PROBE SPEED / TIME", (Vector2){gx, dst.y -10}, PANEL_FONT_SIZE, 1, (Color){60, 60, 60, 255});
 
    DrawTextEx(globalFont, "SPD", (Vector2){dst.x + 2, gy}, 9, 1, (Color){50, 50, 50, 255});
 
    int ticks = 6;
    for (int i = 0; i <= ticks; i++) {
        int tx = gx + (int)((float)i / ticks * gw);
        DrawLine(tx, gy + gh, tx, gy + gh + 3, (Color){40, 40, 40, 255});
    }
 
    for (int i = 0; i <= 4; i++) {
        int ty = gy + (int)((float)i / 4 * gh);
        DrawLine(gx - 3, ty, gx, ty, (Color){40, 40, 40, 255});
    }
 
    if (probeCount == 0) {
        DrawTextEx(globalFont, "NO PROBES", (Vector2){gx + gw/2 - MeasureTextEx(globalFont, "NO PROBES", PANEL_FONT_SIZE, 1).x/2,
                 gy + gh/2 - PANEL_FONT_SIZE/2}, PANEL_FONT_SIZE, 1, (Color){40, 40, 40, 255});
        return;
    }
 
    float maxSpeed = 1.f;
    for (int p = 0; p < probeCount; p++) {
        for (int i = 0; i < probes[p]->historyCount; i++)
            if (probes[p]->speedHistory[i] > maxSpeed)
                maxSpeed = probes[p]->speedHistory[i];
    }
 
    char maxStr[16];
    snprintf(maxStr, sizeof(maxStr), "%.0f", maxSpeed);
    DrawTextEx(globalFont, maxStr, (Vector2){dst.x + 2, gy}, 9, 1, (Color){50, 50, 50, 255});
 
    Probe *last = probes[probeCount - 1];
    if (last->historyCount > 1) {
        float sum = 0;
        for (int i = 0; i < last->historyCount; i++) sum += last->speedHistory[i];
        float avg  = sum / last->historyCount;
        int   avgY = gy + gh - (int)((avg / maxSpeed) * gh);
        for (int x = gx; x < gx + gw; x += 6)
            DrawRectangle(x, avgY, 3, 1, (Color){50, 50, 50, 255});
        char avgStr[16];
        snprintf(avgStr, sizeof(avgStr), "AVG %.0f", avg);
        DrawTextEx(globalFont, avgStr, (Vector2){gx + gw - MeasureTextEx(globalFont, avgStr, 9, 1).x - 2, avgY - 10}, 9, 1, (Color){50, 50, 50, 255});
    }
 
    unsigned char brightnesses[] = { 255, 180, 120, 80, 60, 50, 40, 35 };
 
    for (int p = 0; p < probeCount; p++) {
        Probe *probe = probes[p];
        if (probe->historyCount < 2) continue;
 
        unsigned char br = brightnesses[p % 8];
        Color c = (Color){br, br, br, 255};
 
        for (int i = 0; i < probe->historyCount - 1; i++) {
            int   ai  = (probe->historyIndex - probe->historyCount + i     + SPEED_HISTORY) % SPEED_HISTORY;
            int   bi  = (probe->historyIndex - probe->historyCount + i + 1 + SPEED_HISTORY) % SPEED_HISTORY;
 
            float ax  = gx + (float)i       / (SPEED_HISTORY - 1) * gw;
            float bx  = gx + (float)(i + 1) / (SPEED_HISTORY - 1) * gw;
            float ay  = gy + gh - (probe->speedHistory[ai] / maxSpeed) * gh;
            float by  = gy + gh - (probe->speedHistory[bi] / maxSpeed) * gh;
 
            DrawLineEx((Vector2){ax, ay}, (Vector2){bx, by}, 1.f, c);
        }
 
        float cur = probe->speedHistory[(probe->historyIndex - 1 + SPEED_HISTORY) % SPEED_HISTORY];
        char curStr[16];
        snprintf(curStr, sizeof(curStr), "P%d %.1f", p + 1, cur);
        int labelY = gy + 2 + p * (PANEL_FONT_SIZE + 2);
        DrawTextEx(globalFont, curStr, (Vector2){gx + gw - MeasureTextEx(globalFont, curStr, PANEL_FONT_SIZE, 1).x - 4,
                 labelY}, PANEL_FONT_SIZE, 1, c);
    }
}

