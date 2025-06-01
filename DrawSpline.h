#ifndef DRAW_SPLINE_H
#define DRAW_SPLINE_H

#include <windows.h>
#include "DrawFilling.h"  


void DrawCardinalSpline(HDC hdc, Point P[], int n, double c, COLORREF color);

#endif // DRAW_SPLINE_H 