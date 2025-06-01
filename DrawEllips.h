#ifndef DRAW_ELLIPS_H
#define DRAW_ELLIPS_H

#include <windows.h>

void DrawEllipseDirect(HDC hdc, int x1, int y1, int x2, int y2, COLORREF color);
void DrawEllipsePolar(HDC hdc, int xc, int yc, int a, int b, COLORREF color);
void DrawEllipseMidpoint(HDC hdc, int xc, int yc, int a, int b, COLORREF color);

#endif // DRAW_ELLIPS_H 