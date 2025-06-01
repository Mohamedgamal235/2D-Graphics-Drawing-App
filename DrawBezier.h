#ifndef DRAW_BEZIER_H
#define DRAW_BEZIER_H

#include <windows.h>
#include "DrawFilling.h"

Point BezierEquation(Point p0, Point p1, Point p2, Point p3, double t);
void DrawBezierCurve(HDC hdc, Point p0, Point p1, Point p2, Point p3, int numPoints, COLORREF color);
void FillRectangleWithBezier(HDC hdc, int left, int top, int right, int bottom, COLORREF color);

#endif // DRAW_BEZIER_H 