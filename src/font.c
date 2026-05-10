#include "font.h"
#include <stddef.h>

Font globalFont = {0};

void LoadGlobalFont(const char *path) {
    globalFont = LoadFontEx(path, 32, NULL, 0);
    SetTextureFilter(globalFont.texture, TEXTURE_FILTER_BILINEAR);
}

void UnloadGlobalFont(void) {
    UnloadFont(globalFont);
}
