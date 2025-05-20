#include <windows.h>
#include <bits/stdc++.h>

// Ahmed Mohsen
void DrawLineDDA(HDC hdc, int x1, int y1, int x2, int y2, COLORREF c) {
    int dx = x2 - x1;
    int dy = y2 - y1;

    int steps = std::max(abs(dx), abs(dy));
    if (steps == 0) {
        SetPixel(hdc, x1, y1, c); // Single point
        return;
    }

    double x_inc = dx / (double)steps;
    double y_inc = dy / (double)steps;

    double x = x1;
    double y = y1;
    for (int i = 0; i <= steps; i++) {
        SetPixel(hdc, round(x), round(y), c);
        x += x_inc;
        y += y_inc;
    }
}


// -------------------------------------------

// Anas
void DrawLineMidpoint(HDC hdc, int x1, int y1, int x2, int y2, COLORREF c) {
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = (x2 >= x1) ? 1 : -1;
    int sy = (y2 >= y1) ? 1 : -1;

    int x = x1;
    int y = y1;

    if (dx > dy) {
        int d = 2 * dy - dx;
        for (int i = 0; i <= dx; i++) {
            SetPixel(hdc, x, y, c);
            if (d >= 0) {
                y += sy;
                d -= 2 * dx;
            }
            x += sx;
            d += 2 * dy;
        }
    }
    else {
        int d = 2 * dx - dy;
        for (int i = 0; i <= dy; i++) {
            SetPixel(hdc, x, y, c);
            if (d >= 0) {
                x += sx;
                d -= 2 * dy;
            }
            y += sy;
            d += 2 * dx;
        }
    }
}

// -------------------------------------------

// Osama
void DrawLineParametric(HDC hdc , int x1, int y1, int x2, int y2 , COLORREF c){

}
