#ifndef PANEL_H
#define PANEL_H
#define PANEL_PADDING     10
#define PANEL_LINE_HEIGHT 18
#define PANEL_FONT_SIZE   18
#include <raylib.h>
#include <stdbool.h>

typedef struct {
    Rectangle dst;
    char *title;
    bool active;
    int cursorY;
} Panel;

Panel CreatePanel(float x, float y, float w, float h, char *title);

void BeginPanel(Panel *p);
void EndPanel(Panel *p);

#endif
