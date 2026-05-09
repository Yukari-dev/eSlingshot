#include "border.h"

void DrawOneCornerBorder(Rectangle r, float length, Color c){
    DrawLine(r.x, r.y, r.x+length, r.y, c);
    DrawLine(r.x, r.y, r.x, r.y+length, c);
}

void DrawTwoCornerBorder(Rectangle r, float length, Color c){
    DrawOneCornerBorder(r, length, c);
    
    DrawLine(r.x+r.width, r.y+r.height, r.x+r.width-length, r.y+r.height, c);
    DrawLine(r.x+r.width, r.y+r.height, r.x+r.width, r.y+r.height-length, c);
}

void DrawThreeCornerBorder(Rectangle r, float length, Color c){
    DrawOneCornerBorder(r, length, c);
    DrawTwoCornerBorder(r, length, c);

    DrawLine(r.x+r.width, r.y, r.x+r.width-length, r.y, c);
    DrawLine(r.x+r.width, r.y, r.x+r.width, r.y+length, c);
}

void DrawFourCornerBorder(Rectangle r, float length, Color c){
    DrawOneCornerBorder(r, length, c);
    DrawTwoCornerBorder(r, length, c);
    DrawThreeCornerBorder(r, length, c);
    
    DrawLine(r.x, r.y+r.height, r.x, r.y+r.height-length, c);
    DrawLine(r.x, r.y+r.height, r.x+length, r.y+r.height, c);
}
