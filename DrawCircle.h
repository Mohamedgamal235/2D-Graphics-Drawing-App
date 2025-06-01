#ifndef DRAW_CIRCLE_H
#define DRAW_CIRCLE_H

#include <windows.h>

void DrawCircleDirect(HDC hdc, int xc, int yc, int R, COLORREF color);
void DrawCirclePolar(HDC hdc, int xc, int yc, int R, COLORREF color);
void DrawCircleIterativePolar(HDC hdc, int xc, int yc, int R, COLORREF color);
void DrawCircleMidpoint(HDC hdc, int xc, int yc, int R, COLORREF color);
void DrawCircleModifiedMidpoint(HDC hdc, int xc, int yc, int R, COLORREF color);
void DrawClippingCircle(HDC hdc, int xc, int yc, int r, COLORREF color);

#endif // DRAW_CIRCLE_H 