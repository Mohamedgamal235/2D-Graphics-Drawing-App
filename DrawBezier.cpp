#include "DrawBezier.h"
#include <cmath>
#define M_PI 3.14159265358979323846

Point BezierEquation(Point p0, Point p1, Point p2, Point p3, double t) {
    // (1-t)^3p0 + 3(1-t)^2*tp1 + 3(1-t)*t^2*p2 + t^3 * p3
    double u = 1 - t;
    double u3 = u * u * u;
    double u2 = u * u;
    double t2 = t * t;
    double t3 = t * t * t;

    Point res;
    res.x = (u3 * p0.x) + (3 * u2 * t * p1.x) + (3 * u * t2 * p2.x) + (t3 * p3.x);
    res.y = (u3 * p0.y) + (3 * u2 * t * p1.y) + (3 * u * t2 * p2.y) + (t3 * p3.y);

    return res;
}

void DrawBezierCurve(HDC hdc, Point p0, Point p1, Point p2, Point p3, int numPoints, COLORREF color) {
    for (double t = 0; t <= 1; t += 1.0 / numPoints) {
        Point p = BezierEquation(p0, p1, p2, p3, t);
        SetPixel(hdc, (int)round(p.x), (int)round(p.y), color);
    }
}

void FillRectangleWithBezier(HDC hdc, int left, int top, int right, int bottom, COLORREF color) {
    const int numCurves = 20;  // Number of horizontal curves to draw
    const int numPoints = 100;  // Points per curve
    
    double width = right - left;
    double height = bottom - top;
    
    // Draw multiple horizontal Bezier curves
    for (int i = 0; i <= numCurves; i++) {
        double y = top + (height * i / numCurves);
        
        // Create control points for horizontal curve
        Point p0(left, y);  // Start point
        Point p3(right, y); // End point
        
        // Control points with wave effect
        double waveAmplitude = height / 8;
        double phase = 2 * M_PI * i / numCurves;
        
        // First control point at 1/3 distance
        Point p1(left + width/3, y + waveAmplitude * sin(phase));
        
        // Second control point at 2/3 distance
        Point p2(left + 2*width/3, y - waveAmplitude * sin(phase));
        
        DrawBezierCurve(hdc, p0, p1, p2, p3, numPoints, color);
    }
} 