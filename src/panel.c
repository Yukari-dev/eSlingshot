#include "panel.h"
#include <string.h>

Panel CreatePanel(float x, float y, float w, float h, char *title){
    Panel p = {0};
    p.dst = (Rectangle){x, y, w, h};
    if(title)
        p.title = strdup(title);
    p.active = true;
    p.cursorY = 0;
    return p;
}

void BeginPanel(Panel *p){
    if(!p || !p->active) return;
    p->cursorY = PANEL_PADDING;
    
    // if(p->title){
    //     int titleSize = MeasureText(p->title, PANEL_FONT_SIZE);
    //     DrawText(p->title,
    //         p->bounds.x+p->bounds.width/2.f-titleSize/2.f,
    //         p->bounds.y + p->cursorY,
    //         PANEL_FONT_SIZE, WHITE);
    //     p->cursorY += PANEL_LINE_HEIGHT + 8;
    // }
}

void EndPanel(Panel *p){
    (void)p;
}

