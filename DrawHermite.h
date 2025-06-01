#ifndef DRAW_HERMITE_H
#define DRAW_HERMITE_H

#include <windows.h>
#include "DrawFilling.h"

void DrawHermiteCurve(HDC hdc, HermitePoint p1, HermitePoint p2, int numPoints, COLORREF color);
void FillSquareWithHermite(HDC hdc, int left, int top, int right, int bottom, COLORREF color);

#endif // DRAW_HERMITE_H 