#include <windows.h>
#include <cmath>
#include <algorithm>  // for std::swap
#include "DrawLine.h"
using namespace std ;

// Ahmed Mohsen
void DrawLineDDA(HDC hdc, int x1, int y1, int x2, int y2, COLORREF color) {
    int dx = x2 - x1;
    int dy = y2 - y1;
    SetPixel(hdc, x1, y1, color);
    if (abs(dx) >= abs(dy)) {
        int x = x1;
        double y = y1;
        double m = (double)dy / dx;
        if (dx > 0) {
            while (x < x2) {
                x++;
                y += m;
                SetPixel(hdc, x, round(y), color);
            }
        } else if (dx < 0) {
            while (x > x2) {
                x--;
                y -= m;
                SetPixel(hdc, x, round(y), color);
            }
        }
    } else {
        int y = y1;
        double x = x1;
        double m = (double)dx / dy;
        if (dy > 0) {
            while (y < y2) {
                y++;
                x += m;
                SetPixel(hdc, round(x), y, color);
            }
        } else if (dy < 0) {
            while (y > y2) {
                y--;
                x -= m;
                SetPixel(hdc, round(x), y, color);
            }
        }
    }
}


// -------------------------------------------

// Anas
void DrawLineMidpoint(HDC hdc, int x1, int y1, int x2, int y2, COLORREF color) {
    int dx = x2 - x1;
    int dy = y2 - y1;
    int d, d1, d2;
    int x = x1, y = y1;

    SetPixel(hdc, x1, y1, color);
    if (abs(dx) >= abs(dy)) {
        if (dx < 0) {
            std::swap(x1, x2);
            std::swap(y1, y2);
            dx = -dx;
            dy = -dy;
            x = x1;
            y = y1;
        }
        d = dx - 2 * abs(dy);
        d1 = -2 * abs(dy);
        d2 = 2 * (dx - abs(dy));

        while (x < x2) {
            if (d > 0) {
                d += d1;
            } else {
                d += d2;
                y += (dy > 0) ? 1 : -1;
            }
            x++;
            SetPixel(hdc, x, y, color);
        }
    } else {
        if (dy < 0) {
            std::swap(x1, x2);
            std::swap(y1, y2);
            dx = -dx;
            dy = -dy;
            x = x1;
            y = y1;
        }
        d = dy - 2 * abs(dx);
        d1 = -2 * abs(dx);
        d2 = 2 * (dy - abs(dx));

        while (y < y2) {
            if (d > 0) {
                d += d1;
            } else {
                d += d2;
                x += (dx > 0) ? 1 : -1;
            }
            y++;
            SetPixel(hdc, x, y, color);
        }
    }
}

// -------------------------------------------

// Osama
void DrawLineParametric(HDC hdc, int x1, int y1, int x2, int y2, COLORREF color) {
    double dx = x2 - x1;
    double dy = y2 - y1;
    for (double t = 0; t <= 1; t += 0.001) {
        int x = round(x1 + t * dx);
        int y = round(y1 + t * dy);
        SetPixel(hdc, x, y, color);
    }
}
