#ifndef DRAW_POLYGON_CLIPPING_H
#define DRAW_POLYGON_CLIPPING_H

#include <windows.h>

struct Vertex {
    double x, y;
    Vertex(double x = 0, double y = 0) : x(x), y(y) {}
};


void ClipPolygonRectangle(HDC hdc, Vertex points[], int n, int xleft, int ytop, int xright, int ybottom);

#endif // DRAW_POLYGON_CLIPPING_H 