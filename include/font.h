#ifndef FONT_H
#define FONT_H
#include <raylib.h>

extern Font globalFont;

void LoadGlobalFont(const char *path);
void UnloadGlobalFont(void);

#endif
