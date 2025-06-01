#include <windows.h>
#include "DrawSpline.h"
#include <cmath>

// Mohamed Gamal
void DrawCardinalSpline(HDC hdc, Vector2 P[], int n, double c, int numPoints, COLORREF color) {
    if (n < 3) return;  // Need at least 3 points for cardinal spline

    double c1 = 1 - c;
    // Initial tangent
    Vector2 T0(c1 * (P[2].x - P[0].x), c1 * (P[2].y - P[0].y));

    for (int i = 2; i < n - 1; i++) {
        // Calculate next tangent
        Vector2 T1(c1 * (P[i + 1].x - P[i - 1].x), c1 * (P[i + 1].y - P[i - 1].y));

        // Convert Vector2 points to HermitePoints for the curve segment
        HermitePoint p1(P[i - 1].x, P[i - 1].y, T0.x, T0.y);
        HermitePoint p2(P[i].x, P[i].y, T1.x, T1.y);

        // Draw the curve segment
        DrawHermiteCurve(hdc, p1, p2, numPoints, color);

        // Current tangent becomes the starting tangent for next segment
        T0 = T1;
    }
}