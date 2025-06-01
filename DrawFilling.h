#ifndef DRAW_FILLING_H
#define DRAW_FILLING_H

#include <windows.h>
#include <list>

struct Point {
    int x, y;
    Point(int x = 0, int y = 0) : x(x), y(y) {}
};

struct PolygonPoint {
    int x, y;
    PolygonPoint(int x = 0, int y = 0) : x(x), y(y) {}
};

// Helper function for circle clipping
bool IsPointInsideClippingCircle(int x, int y, int clipXc, int clipYc, int clipR);

// Circle filling functions with clipping
void FillCircleWithLines(HDC hdc, int xc, int yc, int r, int quarter, COLORREF color);
void FillCircleWithCircles(HDC hdc, int xc, int yc, int r, int quarter, int clipXc, int clipYc, int clipR);

// Other filling functions
void FillSquareWithHermite(HDC hdc);
void FillRectangleWithBezier(HDC hdc);
void ConvexFilling(HDC hdc, Point p[], int n, COLORREF color);
void NonConvexFilling(HDC hdc, PolygonPoint points[], int n, COLORREF color);

#endif // DRAW_FILLING_H 