#ifndef DRAW_POINT_CLIPPING_H
#define DRAW_POINT_CLIPPING_H

#include <windows.h>

void ClipPointSquare(HDC hdc, int x, int y, int xleft, int ytop, int xright, int ybottom, COLORREF color, int pointRadius);
void ClipPointRectangle(HDC hdc);

#endif // DRAW_POINT_CLIPPING_H 