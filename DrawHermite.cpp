#include "DrawHermite.h"
#include <cmath>
#define M_PI 3.14159265358979323846

void DrawHermiteCurve(HDC hdc, HermitePoint p1, HermitePoint p2, int numPoints, COLORREF color) {
    // Hermite basis matrix
    const double H[4][4] = {
        {2, -2, 1, 1},
        {-3, 3, -2, -1},
        {0, 0, 1, 0},
        {1, 0, 0, 0}
    };

    // Calculate points along the curve
    for (double t = 0; t <= 1; t += 1.0 / numPoints) {
        // Calculate basis vectors
        double t2 = t * t;
        double t3 = t2 * t;
        double T[4] = {t3, t2, t, 1};
        
        // Calculate x coordinate
        double x = 0;
        double y = 0;
        double gx[4] = {p1.x, p2.x, p1.dx, p2.dx};
        double gy[4] = {p1.y, p2.y, p1.dy, p2.dy};
        
        for (int i = 0; i < 4; i++) {
            double cx = 0;
            double cy = 0;
            for (int j = 0; j < 4; j++) {
                cx += H[i][j] * gx[j];
                cy += H[i][j] * gy[j];
            }
            x += T[i] * cx;
            y += T[i] * cy;
        }
        
        SetPixel(hdc, (int)round(x), (int)round(y), color);
    }
}

void FillSquareWithHermite(HDC hdc, int left, int top, int right, int bottom, COLORREF color) {
    const int numCurves = 20;  // Number of vertical curves to draw
    const int numPoints = 100;  // Points per curve
    
    double width = right - left;
    double height = bottom - top;
    
    // Draw multiple vertical Hermite curves
    for (int i = 0; i <= numCurves; i++) {
        double x = left + (width * i / numCurves);
        
        // Create control points for vertical curve
        // Start point at top with upward tangent
        HermitePoint p1(x, top, 0, height/3);  
        // End point at bottom with downward tangent
        HermitePoint p2(x, bottom, 0, -height/3);  
        
        // Add wave effect based on position
        double waveAmplitude = width / 10;
        double phase = 2 * M_PI * i / numCurves;
        p1.dx = waveAmplitude * sin(phase);
        p2.dx = -waveAmplitude * sin(phase);
        
        DrawHermiteCurve(hdc, p1, p2, numPoints, color);
    }
} 