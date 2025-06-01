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

// Updated function declarations with clipping circle parameters
void FillCircleWithLines(HDC hdc, int xc, int yc, int r, int quarter, COLORREF color, int clipXc, int clipYc, int clipR);
void FillCircleWithCircles(HDC hdc, int xc, int yc, int r, int quarter, int clipXc, int clipYc, int clipR);

void FillSquareWithHermite(HDC hdc);
void FillRectangleWithBezier(HDC hdc);
void ConvexFilling(HDC hdc, Point p[], int n, COLORREF color);
void NonConvexFilling(HDC hdc, PolygonPoint points[], int n, COLORREF color);

#endif // DRAW_FILLING_H 