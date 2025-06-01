#include "DrawFilling.h"
#include "DrawLine.h"
#include "DrawCircle.h"
#include <cmath>

void FillCircleWithLines(HDC hdc, int xc, int yc, int r, int quarter, COLORREF color) {
    // Quarter 1: Top Right
    // Quarter 2: Top Left
    // Quarter 3: Bottom Left
    // Quarter 4: Bottom Right

    switch (quarter) {
        case 1: // Top Right
            for (int x = xc; x <= xc + r; x++) {
                int y1 = yc;
                int y2 = yc - (int) sqrt(r * r - (x - xc) * (x - xc));
                DrawLineMidpoint(hdc, x, y1, x, y2, color);
            }
            break;

        case 2: // Top Left
            for (int x = xc; x >= xc - r; x--) {
                int y1 = yc;
                int y2 = yc - (int) sqrt(r * r - (x - xc) * (x - xc));
                DrawLineMidpoint(hdc, x, y1, x, y2, color);
            }
            break;

        case 3: // Bottom Left
            for (int x = xc; x >= xc - r; x--) {
                int y1 = yc;
                int y2 = yc + (int) sqrt(r * r - (x - xc) * (x - xc));
                DrawLineMidpoint(hdc, x, y1, x, y2, color);
            }
            break;

        case 4: // Bottom Right
            for (int x = xc; x <= xc + r; x++) {
                int y1 = yc;
                int y2 = yc + (int) sqrt(r * r - (x - xc) * (x - xc));
                DrawLineMidpoint(hdc, x, y1, x, y2, color);
            }
            break;
    }
}


void FillCircleWithCircles(HDC hdc, int xc, int yc, int r, int quarter, COLORREF color) {
    int smallR = 6;
    int step = 5;

    for (int dx = -r; dx <= r; dx += step) {
        for (int dy = -r; dy <= r; dy += step) {
            int distSquared = dx * dx + dy * dy;
            if (distSquared <= r * r) {
                bool inQuarter = false;
                switch (quarter) {
                    case 1: inQuarter = (dx >= 0 && dy <= 0);
                        break; // Top Right
                    case 2: inQuarter = (dx <= 0 && dy <= 0);
                        break; // Top Left
                    case 3: inQuarter = (dx <= 0 && dy >= 0);
                        break; // Bottom Left
                    case 4: inQuarter = (dx >= 0 && dy >= 0);
                        break; // Bottom Right
                }

                if (inQuarter) {
                    DrawCircleModifiedMidpoint2(hdc, xc + dx, yc + dy, smallR, xc, yc, r, color);
                }
            }
        }
    }
}


void DrawCircleModifiedMidpoint2(HDC hdc, int xc, int yc, int r, int xc_big, int yc_big, int R_big, COLORREF color) {
    int x = 0, y = r;
    int p = 1 - r;

    while (x <= y) {
        Put8PixelsWithClipping(hdc, xc, yc, x, y, xc_big, yc_big, R_big, color);
        x++;
        if (p < 0)
            p += 2 * x + 1;
        else {
            y--;
            p += 2 * (x - y) + 1;
        }
    }
}


void Put8PixelsWithClipping(HDC hdc, int xc, int yc, int x, int y, int xc_big, int yc_big, int R_big, COLORREF color) {
    PutPixelIfInBigCircle(hdc, xc + x, yc + y, xc_big, yc_big, R_big, color);
    PutPixelIfInBigCircle(hdc, xc - x, yc + y, xc_big, yc_big, R_big, color);
    PutPixelIfInBigCircle(hdc, xc + x, yc - y, xc_big, yc_big, R_big, color);
    PutPixelIfInBigCircle(hdc, xc - x, yc - y, xc_big, yc_big, R_big, color);
    PutPixelIfInBigCircle(hdc, xc + y, yc + x, xc_big, yc_big, R_big, color);
    PutPixelIfInBigCircle(hdc, xc - y, yc + x, xc_big, yc_big, R_big, color);
    PutPixelIfInBigCircle(hdc, xc + y, yc - x, xc_big, yc_big, R_big, color);
    PutPixelIfInBigCircle(hdc, xc - y, yc - x, xc_big, yc_big, R_big, color);
}

void PutPixelIfInBigCircle(HDC hdc, int x, int y, int xc_big, int yc_big, int R, COLORREF color) {
    int dx = x - xc_big;
    int dy = y - yc_big;
    if (dx * dx + dy * dy <= R * R)
        SetPixel(hdc, x, y, color);
}
