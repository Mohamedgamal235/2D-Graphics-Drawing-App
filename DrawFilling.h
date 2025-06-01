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

void FillCircleWithLines(HDC hdc, int xc, int yc, int r, int quarter, COLORREF color);
void FillCircleWithCircles(HDC hdc, int xc, int yc, int r, int quarter);
void FillSquareWithHermite(HDC hdc);
void FillRectangleWithBezier(HDC hdc);
void ConvexFilling(HDC hdc, Point p[], int n, COLORREF color);
void NonConvexFilling(HDC hdc, PolygonPoint points[], int n, COLORREF color);

#endif // DRAW_FILLING_H 