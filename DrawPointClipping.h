#ifndef DRAW_POINT_CLIPPING_H
#define DRAW_POINT_CLIPPING_H

#include <windows.h>

void ClipPointRectangle(HDC hdc, int x, int y, int xleft, int ytop, int xright, int ybottom, COLORREF color);
void ClipPointSquare(HDC hdc, int x, int y, int xleft, int ytop, int xright, int ybottom, COLORREF color);

#endif // DRAW_POINT_CLIPPING_H 