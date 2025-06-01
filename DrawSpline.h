#ifndef DRAW_SPLINE_H
#define DRAW_SPLINE_H

#include <windows.h>
#include "DrawFilling.h"
#include "DrawHermite.h"

struct Vector2 {
    double x, y;
    Vector2(double x = 0, double y = 0) : x(x), y(y) {}
};

void DrawCardinalSpline(HDC hdc, Vector2 P[], int n, double c, int numPoints, COLORREF color);

#endif // DRAW_SPLINE_H 