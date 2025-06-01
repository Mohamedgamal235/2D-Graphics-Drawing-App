#include <windows.h>
#include "DrawSpline.h"
#include <cmath>

// Mohamed Gamal
void DrawCardinalSpline(HDC hdc, Vector2 P[], int n, double c, int numPoints, COLORREF color) {
    if (n < 3) return;

    double c1 = 1 - c;
    Vector2 T0(c1 * (P[2].x - P[0].x), c1 * (P[2].y - P[0].y));

    for (int i = 2; i < n - 1; i++) {
        Vector2 T1(c1 * (P[i + 1].x - P[i - 1].x), c1 * (P[i + 1].y - P[i - 1].y));

        HermitePoint p1(P[i - 1].x, P[i - 1].y, T0.x, T0.y);
        HermitePoint p2(P[i].x, P[i].y, T1.x, T1.y);

        DrawHermiteCurve(hdc, p1, p2, numPoints, color);

        T0 = T1;
    }
}