#ifndef DRAW_LINE_CLIPPING_H
#define DRAW_LINE_CLIPPING_H

#include <windows.h>

union OutCode{
    unsigned all : 4;
    struct{
        unsigned left : 1, right : 1,top : 1, bottom : 1;
    };
};

void ClipLineRectangle(HDC hdc, int x1, int y1, int x2, int y2, int xleft, int ytop, int xright, int ybottom);
void ClipLineSquare(HDC hdc, int x1, int y1, int x2, int y2, int xleft, int ytop, int xright, int ybottom);

#endif // DRAW_LINE_CLIPPING_H 