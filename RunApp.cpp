#include <utility>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <windef.h>
#include <cmath>
#include <vector>
#include <fstream>
#include <iostream>
#include "DrawLine.h"
#include "DrawCircle.h"
#include "DrawEllips.h"
#include "DrawFilling.h"
#include "DrawFloodFill.h"
#include "DrawWindow.h"
#include "DrawPolygonClipping.h"
#include "DrawPointClipping.h"
#include "DrawLineClipping.h"
#include "DrawSpline.h"
#include "DrawHermite.h"
#include "DrawBezier.h"

using namespace std;

enum DrawShap {
    NONE,
    LINE_DDA,
    LINE_MIDPOINT,
    LINE_PARAMETRIC,
    CIRCLE_DIRECT,
    CIRCLE_POLAR,
    CIRCLE_ITERATIVE_POLAR,
    CIRCLE_MIDPOINT,
    CIRCEL_MODIFIED_MIDPOINT,
    FILL_CIRCLE_WITH_LINE,
    FILL_CIRCLE_WITH_CIRCLE,
    FILL_HERMITE,
    FILL_BEZIER,
    FILL_CONVEX,
    FILL_NONCONVEX,
    FLOOD_FILL_RECURSIVE,
    FLOOD_FILL_NONRECURSIVE,
    SPLINE_CARDINAL,
    ELLIPSE_DIRECT,
    ELLIPSE_POLAR,
    ELLIPSE_MIDPOINT,
    CLIP_POINT_RECT,
    CLIP_LINE_RECT,
    CLIP_POLYGON_RECT,
    CLIP_POINT_SQUARE,
    CLIP_LINE_SQUARE,
    RED,
    GREEN,
    BLUE,
    BLACK,
    CLEAR_SCREEN,
    SAVE_SCREEN,
    LOAD_SCREEN
};

//------------------------------------ Save and Load --------------------------------------//
struct Shape {
    Point start;
    Point end;
    COLORREF color;
    DrawShap algorithm;
    
    // FILL WITH LINES
    pair<int, COLORREF> quartersLines[4] = {
        make_pair(0, RGB(0, 0, 0)),
        make_pair(0, RGB(0, 0, 0)),
        make_pair(0, RGB(0, 0, 0)),
        make_pair(0, RGB(0, 0, 0))
    };      
    

    // FILL WITH CIRCLES
    pair<int, COLORREF> quartersCircles[4] = {
        make_pair(0, RGB(0, 0, 0)),
        make_pair(0, RGB(0, 0, 0)),
        make_pair(0, RGB(0, 0, 0)),
        make_pair(0, RGB(0, 0, 0))
    };      

    vector<Point> extraPoints;              // For spline points
    vector<Point> splinePoints;
    double tension;                         // For Cardinal spline
    COLORREF targetColorRecursive;          // For flood fill Rec 
    COLORREF targetColorNonRecursive;      // For flood fill Non Rec 
    
};

vector<Shape> shapes;
HDC hdc;
PolygonPoint points[6];
int p_index = 0;
Point pointsConvex[5];
Vertex pointsPolygonClipping[6] = {Vertex(0,0), Vertex(0,0), Vertex(0,0), Vertex(0,0), Vertex(0,0), Vertex(0,0)};


Point startPoint;
DrawShap currShape = NONE;
COLORREF currColor = RGB(0, 0, 0); // black
bool isDrawing = false;


bool circleisDrawn = false;
int cir_r_lines = 0 ;
int cir_x_line = 0 ;
int cir_y_line  = 0 ;

bool circleisDrawnWithCircle = false;
int cir_r_with_circle = 0;
int cir_x_with_circle = 0;
int cir_y_with_circle = 0;


// Add after other global variables
Vector2 splinePoints[100];  // Array to store spline points
int splinePointCount = 0;   // Counter for spline points
const double CARDINAL_C = 0.5;  // Tension parameter for cardinal spline

// Add this with other global variables at the top
bool shouldClear = false;

void SaveToFile(const char* filename) {
    ofstream out(filename);
    if (!out.is_open()) {
        MessageBoxA(NULL, "Failed to open file for saving!", "Error", MB_ICONERROR);
        return;
    }

    // First save the total number of shapes
    out << shapes.size() << '\n';

    for (const auto& s : shapes) {
        // Save algorithm type first to know how to handle the rest
        out << static_cast<int>(s.algorithm) << ' ';
        
        // Save shape-specific data based on algorithm
        switch (s.algorithm) {
            case LINE_DDA:
            case LINE_MIDPOINT:
            case LINE_PARAMETRIC: {
                // For lines, just save start and end points
                out << s.start.x << ' ' << s.start.y << ' '
                    << s.end.x << ' ' << s.end.y << ' '
                    << s.color << '\n';
                break;
            }
            case CIRCLE_DIRECT:
            case CIRCLE_POLAR:
            case CIRCLE_ITERATIVE_POLAR:
            case CIRCLE_MIDPOINT:
            case CIRCEL_MODIFIED_MIDPOINT: {
                // For circles, save center and radius point
                out << s.start.x << ' ' << s.start.y << ' '
                    << s.end.x << ' ' << s.end.y << ' '
                    << s.color << '\n';
                break;
            }
            case FILL_CIRCLE_WITH_LINE: {
                out << s.start.x << ' ' << s.start.y << ' '
                    << s.end.x << ' ' << s.end.y << ' '
                    << s.color << ' '
                    << cir_r_lines << ' '
                    << cir_x_line << ' '
                    << cir_y_line << ' '
                    << s.quartersLines[0].first << ' ' << s.quartersLines[0].second << ' '
                    << s.quartersLines[1].first << ' ' << s.quartersLines[1].second << ' '
                    << s.quartersLines[2].first << ' ' << s.quartersLines[2].second << ' '
                    << s.quartersLines[3].first << ' ' << s.quartersLines[3].second << '\n';
                break;
            }
            case FILL_CIRCLE_WITH_CIRCLE: {
                out << s.start.x << ' ' << s.start.y << ' '
                    << s.end.x << ' ' << s.end.y << ' '
                    << s.color << ' '
                    << cir_r_with_circle << ' '
                    << cir_x_with_circle << ' '
                    << cir_y_with_circle << ' '
                    << s.quartersCircles[0].first << ' ' << s.quartersCircles[0].second << ' '
                    << s.quartersCircles[1].first << ' ' << s.quartersCircles[1].second << ' '
                    << s.quartersCircles[2].first << ' ' << s.quartersCircles[2].second << ' '
                    << s.quartersCircles[3].first << ' ' << s.quartersCircles[3].second << '\n';
                break;
            }
            case SPLINE_CARDINAL: {
                // Save spline parameters
                out << s.start.x << ' ' << s.start.y << ' '
                    << s.color << ' '
                    << s.tension << ' '
                    << s.splinePoints.size() << ' ';
                for (const auto& p : s.splinePoints) 
                    out << p.x << ' ' << p.y << ' ';
                
                out << '\n';
                break;
            }
            case FILL_CONVEX:
            case FILL_NONCONVEX: {
                // Save polygon points
                out << s.start.x << ' ' << s.start.y << ' '
                    << s.end.x << ' ' << s.end.y << ' '
                    << s.color << ' '
                    << s.extraPoints.size() << ' ';
                for (const auto& p : s.extraPoints) {
                    out << p.x << ' ' << p.y << ' ';
                }
                out << '\n';
                break;
            }
            case FLOOD_FILL_RECURSIVE: {
                // Save flood fill parameters
                out << s.start.x << ' ' << s.start.y << ' '
                    << s.color << ' '
                    << s.targetColorRecursive << '\n';
                break;
            }
            case FLOOD_FILL_NONRECURSIVE: {
                // Save flood fill parameters
                out << s.start.x << ' ' << s.start.y << ' '
                    << s.color << ' '
                    << s.targetColorNonRecursive << '\n';
                break;
            }
            case CLIP_POLYGON_RECT: {
                int sz = s.extraPoints.size();
                for (int i = 0 ; i < sz ; i++)
                    out << s.extraPoints[i].x << ' ' << s.extraPoints[i].y << ' ';
                
                break;
            }
            default: {
                // For all other shapes, save basic parameters
                out << s.start.x << ' ' << s.start.y << ' '
                    << s.end.x << ' ' << s.end.y << ' '
                    << s.color << '\n';
                break;
            }
        }
    }
    out.close();
    MessageBoxA(NULL, "Shapes saved successfully.", "Success", MB_OK);
}



void LoadFromFile(const char* filename, HWND hwnd) {
    ifstream in(filename);
    if (!in.is_open()) {
        MessageBoxA(NULL, "Failed to open file for loading!", "Error", MB_ICONERROR);
        return;
    }

    shapes.clear();
    int totalShapes;
    in >> totalShapes;

    for (int i = 0; i < totalShapes; i++) {
        int alg;
        in >> alg;
        DrawShap algorithm = static_cast<DrawShap>(alg);
        
        Shape s;
        s.algorithm = algorithm;

        switch (algorithm) {
            case LINE_DDA:
            case LINE_MIDPOINT:
            case LINE_PARAMETRIC: {
                in >> s.start.x >> s.start.y
                   >> s.end.x >> s.end.y
                   >> s.color;
                break;
            }
            case CIRCLE_DIRECT:
            case CIRCLE_POLAR:
            case CIRCLE_ITERATIVE_POLAR:
            case CIRCLE_MIDPOINT:
            case CIRCEL_MODIFIED_MIDPOINT: {
                in >> s.start.x >> s.start.y
                   >> s.end.x >> s.end.y
                   >> s.color;
                break;
            }
            case FILL_CIRCLE_WITH_LINE: {
                in >> s.start.x >> s.start.y
                   >> s.end.x >> s.end.y
                   >> s.color
                   >> cir_r_lines >> cir_x_line >> cir_y_line;
                
                // Load quarters state and colors
                for(int i = 0; i < 4; i++) {
                    in >> s.quartersLines[i].first >> s.quartersLines[i].second;
                }
                
                // Draw the circle and filled quarters
                DrawCircleMidpoint(hdc, s.start.x, s.start.y, cir_r_lines, s.color);
                for(int i = 0; i < 4; i++) {
                    if(s.quartersLines[i].first) {
                        FillCircleWithLines(hdc, cir_x_line, cir_y_line, 
                                          cir_r_lines, i + 1, s.quartersLines[i].second);
                    }
                }
                break;
            }
            case FILL_CIRCLE_WITH_CIRCLE: {
                in >> s.start.x >> s.start.y
                   >> s.end.x >> s.end.y
                   >> s.color
                   >> cir_r_with_circle >> cir_x_with_circle >> cir_y_with_circle;
                
                // Load quarters state and colors
                for(int i = 0; i < 4; i++) {
                    in >> s.quartersCircles[i].first >> s.quartersCircles[i].second;
                }
                
                // Draw the circle and filled quarters
                DrawCircleMidpoint(hdc, s.start.x, s.start.y, cir_r_with_circle, s.color);
                for(int i = 0; i < 4; i++) {
                    if(s.quartersCircles[i].first) {
                        FillCircleWithCircles(hdc, cir_x_with_circle, cir_y_with_circle, 
                                            cir_r_with_circle, i + 1, s.quartersCircles[i].second);
                    }
                }
                break;
            }
            case SPLINE_CARDINAL: {
                // Load spline parameters
                in >> s.start.x >> s.start.y
                   >> s.color
                   >> s.tension;
                
                // Load spline points
                int pointCount;
                in >> pointCount;
                s.splinePoints.clear();
                
                for (int j = 0; j < pointCount; j++) {
                    Point p;
                    in >> p.x >> p.y;
                    s.splinePoints.push_back(p);
                }
                
                // Convert points to Vector2 array and draw
                if (s.splinePoints.size() >= 3) {
                    Vector2* points = new Vector2[s.splinePoints.size()];
                    for (size_t i = 0; i < s.splinePoints.size(); i++) {
                        points[i].x = s.splinePoints[i].x;
                        points[i].y = s.splinePoints[i].y;
                    }
                    DrawCardinalSpline(hdc, points, s.splinePoints.size(), s.tension, 100, s.color);
                    delete[] points;
                }
                break;
            }
            case FILL_CONVEX:{
                in >> s.start.x >> s.start.y
                   >> s.end.x >> s.end.y
                   >> s.color;
                int pointCount;
                in >> pointCount;
                for (int j = 0; j < pointCount; j++) {
                    Point p;
                    in >> p.x >> p.y;
                    s.extraPoints.push_back(p);
                }
                break;
            }
            case FILL_NONCONVEX: {
                in >> s.start.x >> s.start.y
                   >> s.end.x >> s.end.y
                   >> s.color;
                int pointCount;
                in >> pointCount;
                for (int j = 0; j < pointCount; j++) {
                    Point p;
                    in >> p.x >> p.y;
                    s.extraPoints.push_back(p);
                }
                break;
            }
            case FLOOD_FILL_RECURSIVE: {
                in >> s.start.x >> s.start.y
                   >> s.color
                   >> s.targetColorRecursive;
                break;
            }
            case FLOOD_FILL_NONRECURSIVE: {
                in >> s.start.x >> s.start.y
                   >> s.color
                   >> s.targetColorNonRecursive;
                break;
            }
            case CLIP_POLYGON_RECT: {
                int sz = s.extraPoints.size();
                Vertex* vertices = new Vertex[sz];

                for (int i = 0 ; i < sz ; i++)
                    vertices[i].x = s.extraPoints[i].x, vertices[i].y = s.extraPoints[i].y;
                
                DrawRectungle(hdc, 200, 100, 800, 300);
                ClipPolygonRectangle(hdc, vertices, sz, 200, 100, 800, 300);
                break;
            }
            default: {
                in >> s.start.x >> s.start.y
                   >> s.end.x >> s.end.y
                   >> s.color;
                break;
            }
        }
        shapes.push_back(s);
    }

    in.close();
    shouldClear = false;
    InvalidateRect(hwnd, NULL, TRUE);
}

// ------------------------------------- End of Save and Load ------------------------------------//




// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------

void creatDrawingMenu(HWND hwnd) {
    HMENU hMenu = CreateMenu();
    HMENU hLineMenu = CreatePopupMenu();
    HMENU hCircleMenu = CreatePopupMenu();
    HMENU hEllipseMenu = CreatePopupMenu();
    HMENU hFillMenu = CreatePopupMenu();
    HMENU hFloodMenu = CreatePopupMenu();
    HMENU hClipMenu = CreatePopupMenu();
    HMENU hColorMenu = CreatePopupMenu();

    // For Line
    AppendMenu(hLineMenu, MF_STRING, LINE_DDA, "DDA");
    AppendMenu(hLineMenu, MF_STRING, LINE_MIDPOINT, "Midpoint");
    AppendMenu(hLineMenu, MF_STRING, LINE_PARAMETRIC, "Parametric");
    AppendMenu(hMenu, MF_POPUP, (UINT_PTR) hLineMenu, "Lines");
    //-----------------------------------
    // Circle
    AppendMenu(hCircleMenu, MF_STRING, CIRCLE_DIRECT, "Direct");
    AppendMenu(hCircleMenu, MF_STRING, CIRCLE_POLAR, "Polar");
    AppendMenu(hCircleMenu, MF_STRING, CIRCLE_ITERATIVE_POLAR, "Iterative Polar");
    AppendMenu(hCircleMenu, MF_STRING, CIRCLE_MIDPOINT, "Midpoint");
    AppendMenu(hCircleMenu, MF_STRING, CIRCEL_MODIFIED_MIDPOINT, "Modified Midpoint");
    AppendMenu(hMenu, MF_POPUP, (UINT_PTR) hCircleMenu, "Circles");
    //-----------------------------------
    // Ellips
    AppendMenu(hEllipseMenu, MF_STRING, ELLIPSE_DIRECT, "Direct");
    AppendMenu(hEllipseMenu, MF_STRING, ELLIPSE_POLAR, "Polar");
    AppendMenu(hEllipseMenu, MF_STRING, ELLIPSE_MIDPOINT, "Midpoint");
    AppendMenu(hMenu, MF_POPUP, (UINT_PTR) hEllipseMenu, "Ellipses");
    //-----------------------------------
    // Filling
    AppendMenu(hFillMenu, MF_STRING, FILL_CIRCLE_WITH_LINE, "Circle Fill With Lines");
    AppendMenu(hFillMenu, MF_STRING, FILL_CIRCLE_WITH_CIRCLE, "Circle Fill With Circles");
    AppendMenu(hFillMenu, MF_STRING, FILL_HERMITE, "Square Fill With Hermite");
    AppendMenu(hFillMenu, MF_STRING, FILL_BEZIER, "Rectangle Fill With Bezier");
    AppendMenu(hFillMenu, MF_STRING, FILL_CONVEX, "Convex Filling");
    AppendMenu(hFillMenu, MF_STRING, FILL_NONCONVEX, "Non-Convex Filling");
    AppendMenu(hMenu, MF_POPUP, (UINT_PTR) hFillMenu, "Filling");
    //-----------------------------------
    // Flood Fill
    AppendMenu(hFloodMenu, MF_STRING, FLOOD_FILL_RECURSIVE, "Flood Fill Recursive");
    AppendMenu(hFloodMenu, MF_STRING, FLOOD_FILL_NONRECURSIVE, "Flood Fill Non-Recursive");
    AppendMenu(hMenu, MF_POPUP, (UINT_PTR) hFloodMenu, "Flood Fill");
    //-----------------------------------
    // Spline
    AppendMenu(hMenu, MF_STRING, SPLINE_CARDINAL, "Cardinal Spline");
    //-----------------------------------
    // Clipping
    AppendMenu(hClipMenu, MF_STRING, CLIP_POINT_RECT, "Point Clip (Rect)");
    AppendMenu(hClipMenu, MF_STRING, CLIP_LINE_RECT, "Line Clip (Rect)");
    AppendMenu(hClipMenu, MF_STRING, CLIP_POLYGON_RECT, "Polygon Clip (Rect)");
    AppendMenu(hClipMenu, MF_STRING, CLIP_POINT_SQUARE, "Point Clip (Square)");
    AppendMenu(hClipMenu, MF_STRING, CLIP_LINE_SQUARE, "Line Clip (Square)");
    AppendMenu(hMenu, MF_POPUP, (UINT_PTR) hClipMenu, "Clipping");
    //-----------------------------------
    // Color
    AppendMenu(hColorMenu, MF_STRING, RED, "Red");
    AppendMenu(hColorMenu, MF_STRING, GREEN, "Green");
    AppendMenu(hColorMenu, MF_STRING, BLUE, "Blue");
    AppendMenu(hColorMenu, MF_STRING, BLACK, "Black");
    AppendMenu(hMenu, MF_POPUP, (UINT_PTR) hColorMenu, "Color");
    //-----------------------------------

    AppendMenu(hMenu, MF_STRING, CLEAR_SCREEN, "Clear Screen");
    AppendMenu(hMenu, MF_STRING, SAVE_SCREEN, "Save");
    AppendMenu(hMenu, MF_STRING, LOAD_SCREEN, "Load");

    SetMenu(hwnd, hMenu);
}

// ------------------------------------- Start of Drawing Functions ------------------------------------//
// ------------------------------------- Start of Drawing Functions ------------------------------------//
// ------------------------------------- Start of Drawing Functions ------------------------------------//
// ----------------------------------------------------------------------------------------------------//
// ----------------------------------------------------------------------------------------------------//
// ----------------------------------------------------------------------------------------------------//





void DrawPoint(HDC hdc, int x, int y, COLORREF color) {
    HBRUSH hBrush = CreateSolidBrush(color);
    HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
    Ellipse(hdc, x - 2, y - 2, x + 2, y + 2);
    SelectObject(hdc, oldBrush);
    DeleteObject(hBrush);
}


// ----------------------------------------------------------------------------------------------------//
// ----------------------------------------------------------------------------------------------------//
// ----------------------------------------------------------------------------------------------------//
// ----------------------------------------------------------------------------------------------------//


LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    static POINT startPt;
    static bool isDrawing = false;

    PAINTSTRUCT ps;

    switch (uMsg) {
        case WM_CREATE:
            creatDrawingMenu(hwnd);
            break;

        case WM_PAINT: {
            hdc = BeginPaint(hwnd, &ps);

            if (!shouldClear) {
                for (const auto& s : shapes) {
                    switch (s.algorithm) {
                        case LINE_DDA:
                            DrawLineDDA(hdc, s.start.x, s.start.y, s.end.x, s.end.y, s.color);
                            break;
                        case LINE_MIDPOINT:
                            DrawLineMidpoint(hdc, s.start.x, s.start.y, s.end.x, s.end.y, s.color);
                            break;
                        case LINE_PARAMETRIC:
                            DrawLineParametric(hdc, s.start.x, s.start.y, s.end.x, s.end.y, s.color);
                            break;
                        case CIRCLE_DIRECT: {
                            int R = (int)sqrt((s.end.x - s.start.x) * (s.end.x - s.start.x) + 
                                            (s.end.y - s.start.y) * (s.end.y - s.start.y));
                            DrawCircleDirect(hdc, s.start.x, s.start.y, R, s.color);
                            break;
                        }
                        case CIRCLE_POLAR: {
                            int R = (int)sqrt((s.end.x - s.start.x) * (s.end.x - s.start.x) + 
                                            (s.end.y - s.start.y) * (s.end.y - s.start.y));
                            DrawCirclePolar(hdc, s.start.x, s.start.y, R, s.color);
                            break;
                        }
                        case CIRCLE_ITERATIVE_POLAR: {
                            int R = (int)sqrt((s.end.x - s.start.x) * (s.end.x - s.start.x) + 
                                            (s.end.y - s.start.y) * (s.end.y - s.start.y));
                            DrawCircleIterativePolar(hdc, s.start.x, s.start.y, R, s.color);
                            break;
                        }
                        case CIRCLE_MIDPOINT: {
                            int R = (int)sqrt((s.end.x - s.start.x) * (s.end.x - s.start.x) + 
                                            (s.end.y - s.start.y) * (s.end.y - s.start.y));
                            DrawCircleMidpoint(hdc, s.start.x, s.start.y, R, s.color);
                            break;
                        }
                        case CIRCEL_MODIFIED_MIDPOINT: {
                            int R = (int)sqrt((s.end.x - s.start.x) * (s.end.x - s.start.x) + 
                                            (s.end.y - s.start.y) * (s.end.y - s.start.y));
                            DrawCircleModifiedMidpoint(hdc, s.start.x, s.start.y, R, s.color);
                            break;
                        }
                        case FILL_CIRCLE_WITH_LINE: {
                            int R = (int)sqrt((s.end.x - s.start.x) * (s.end.x - s.start.x) + 
                                            (s.end.y - s.start.y) * (s.end.y - s.start.y));
                            DrawCircleMidpoint(hdc, s.start.x, s.start.y, R, s.color);
                            // Draw all filled quarters
                            for(int i = 0; i < 4; i++) {
                                if(s.quartersLines[i].first == 1) {
                                    FillCircleWithLines(hdc, s.start.x, s.start.y, R, i + 1, s.quartersLines[i].second);
                                }
                            }
                            break;
                        }
                        case FILL_CIRCLE_WITH_CIRCLE: {
                            int R = (int)sqrt((s.end.x - s.start.x) * (s.end.x - s.start.x) + 
                                            (s.end.y - s.start.y) * (s.end.y - s.start.y));
                            DrawCircleMidpoint(hdc, s.start.x, s.start.y, R, s.color);
                            // Draw all filled quarters
                            for(int i = 0; i < 4; i++) {
                                if(s.quartersCircles[i].first == 1) {
                                    FillCircleWithCircles(hdc, s.start.x, s.start.y, R, i + 1, s.quartersCircles[i].second);
                                }
                            }
                            break;
                        }
                        case SPLINE_CARDINAL: {
                            if (s.splinePoints.size() >= 3) {
                                Vector2* points = new Vector2[s.splinePoints.size()];
                                for (size_t i = 0; i < s.splinePoints.size(); i++) {
                                    points[i].x = s.splinePoints[i].x;
                                    points[i].y = s.splinePoints[i].y;
                                }
                                DrawCardinalSpline(hdc, points, s.splinePoints.size(), s.tension, 100, s.color);
                                delete[] points;
                            }
                            break;
                        }

                        case FILL_HERMITE: {
                            int left = min(s.start.x, s.end.x);
                            int right = max(s.start.x, s.end.x);
                            int top = min(s.start.y, s.end.y);
                            int bottom = max(s.start.y, s.end.y);
                            int width = right - left;
                            int height = bottom - top;
                            int size = max(width, height);
                            
                            if (width < height) {
                                int expansion = (height - width) / 2;
                                left -= expansion;
                                right = left + size;
                            } else if (height < width) {
                                int expansion = (width - height) / 2;
                                top -= expansion;
                                bottom = top + size;
                            }
                            
                            HPEN pen = CreatePen(PS_SOLID, 1, s.color);
                            HPEN oldPen = (HPEN)SelectObject(hdc, pen);
                            MoveToEx(hdc, left, top, NULL);
                            LineTo(hdc, right, top);
                            LineTo(hdc, right, bottom);
                            LineTo(hdc, left, bottom);
                            LineTo(hdc, left, top);
                            SelectObject(hdc, oldPen);
                            DeleteObject(pen);
                            
                            FillSquareWithHermite(hdc, left, top, right, bottom, s.color);
                            break;
                        }
                        case FILL_BEZIER: {
                            int left = min(s.start.x, s.end.x);
                            int right = max(s.start.x, s.end.x);
                            int top = min(s.start.y, s.end.y);
                            int bottom = max(s.start.y, s.end.y);
                            
                            HPEN pen = CreatePen(PS_SOLID, 1, s.color);
                            HPEN oldPen = (HPEN)SelectObject(hdc, pen);
                            MoveToEx(hdc, left, top, NULL);
                            LineTo(hdc, right, top);
                            LineTo(hdc, right, bottom);
                            LineTo(hdc, left, bottom);
                            LineTo(hdc, left, top);
                            SelectObject(hdc, oldPen);
                            DeleteObject(pen);
                            
                            FillRectangleWithBezier(hdc, left, top, right, bottom, s.color);
                            break;
                        }
                        case FILL_CONVEX:
                            if (!s.extraPoints.empty()) {
                                Point* points = new Point[s.extraPoints.size()];
                                for (size_t i = 0; i < s.extraPoints.size(); i++) {
                                    points[i] = s.extraPoints[i];
                                }
                                ConvexFilling(hdc, points, s.extraPoints.size(), s.color);
                                delete[] points;
                            }
                            break;
                        case FILL_NONCONVEX:
                            if (!s.extraPoints.empty()) {
                                PolygonPoint* points = new PolygonPoint[s.extraPoints.size()];
                                for (size_t i = 0; i < s.extraPoints.size(); i++) {
                                    points[i].x = s.extraPoints[i].x;
                                    points[i].y = s.extraPoints[i].y;
                                }
                                NonConvexFilling(hdc, points, s.extraPoints.size(), s.color);
                                delete[] points;
                            }
                            break;
                        case FLOOD_FILL_RECURSIVE:
                            RecursiveFloodFill(hdc, s.start.x, s.start.y, s.targetColorRecursive, s.color);
                            break;
                        case FLOOD_FILL_NONRECURSIVE:
                            NonRecursiveFloodFill(hdc, s.start.x, s.start.y, s.targetColorNonRecursive, s.color);
                            break;
                        case ELLIPSE_DIRECT: {
                            DrawEllipseDirect(hdc, s.start.x, s.start.y, s.end.x, s.end.y, s.color);
                            break;
                        }
                        case ELLIPSE_POLAR: {
                            int xc = (s.start.x + s.end.x) / 2;
                            int yc = (s.start.y + s.end.y) / 2;
                            int a = abs(s.end.x - s.start.x) / 2;  // horizontal radius
                            int b = abs(s.end.y - s.start.y) / 2;  // vertical radius
                            DrawEllipsePolar(hdc, xc, yc, a, b, s.color);
                            break;
                        }
                        case ELLIPSE_MIDPOINT: {
                            int xc = (s.start.x + s.end.x) / 2;
                            int yc = (s.start.y + s.end.y) / 2;
                            int a = abs(s.end.x - s.start.x) / 2;  // horizontal radius
                            int b = abs(s.end.y - s.start.y) / 2;  // vertical radius
                            DrawEllipseMidpoint(hdc, xc, yc, a, b, s.color);
                            break;
                        }
                        case CLIP_LINE_RECT: {
                            ClipLineRectangle(hdc, s.start.x, s.start.y, s.end.x, s.end.y, 200, 100, 800, 300);
                            break;
                        }
                        case CLIP_LINE_SQUARE: {
                            ClipLineSquare(hdc, s.start.x, s.start.y, s.end.x, s.end.y, 200, 200, 500, 500);
                            break;
                        }
                        case CLIP_POINT_SQUARE: {
                            ClipPointSquare(hdc, s.start.x, s.start.y, 200, 200, 500, 500, s.color);
                            break;
                        }
                        case CLIP_POINT_RECT: {
                            ClipPointRectangle(hdc, s.start.x, s.start.y, 200, 100, 800, 300, s.color);
                            break;
                        }
                        case CLIP_POLYGON_RECT: {
                            
                            
                            break;
                        }
                    }
                }
            }


            
            // Draw current in-progress shapes
            if (currShape == FILL_CONVEX) {
                for (int i = 0; i < p_index; i++) {
                    DrawPoint(hdc, pointsConvex[i].x, pointsConvex[i].y, currColor);
                }
            }
            else if (currShape == FILL_NONCONVEX) {
                for (int i = 0; i < p_index; i++) {
                    DrawPoint(hdc, points[i].x, points[i].y, currColor);
                }
            }
            
            EndPaint(hwnd, &ps);
            break;
        }

        // ------------------------------------- Start of Drawing Functions ------------------------------------//
        // ------------------------------------- Start of Drawing Functions ------------------------------------//  
        // ------------------------------------- Start of Drawing Functions ------------------------------------//
        // ------------------------------------- Start of Drawing Functions ------------------------------------//

        case WM_COMMAND: {
            DrawShap cmd = (DrawShap) (LOWORD(wParam));
            if (cmd >= RED && cmd <= BLACK) {
                switch (cmd) {
                    case RED: currColor = RGB(255, 0, 0);
                        break;
                    case GREEN: currColor = RGB(0, 255, 0);
                        break;
                    case BLUE: currColor = RGB(0, 0, 255);
                        break;
                    case BLACK: currColor = RGB(0, 0, 0);
                        break;
                }
            } else if (cmd == CLEAR_SCREEN) {
                shapes.clear();  // Clear all saved shapes
                shouldClear = true;  // Set the clear flag
                InvalidateRect(hwnd, NULL, TRUE);
                p_index = 0;
                circleisDrawn = false;
                splinePointCount = 0;
            }
            else if (cmd == SAVE_SCREEN) {
                SaveToFile("shapes.txt");
            }
            else if (cmd == LOAD_SCREEN) {
                LoadFromFile("shapes.txt", hwnd);
                shouldClear = false;  // Reset clear flag when loading
                InvalidateRect(hwnd, NULL, TRUE);
            }
            else {
                currShape = cmd;
                shouldClear = false;  // Reset clear flag when changing shape
                if (cmd != SPLINE_CARDINAL) {
                    splinePointCount = 0;
                }
                if (cmd == CLIP_LINE_RECT || cmd == CLIP_POINT_RECT || cmd == CLIP_POLYGON_RECT){
                    HDC hdc = GetDC(hwnd);
                    DrawRectungle(hdc, 200, 100, 800, 300);
                    ReleaseDC(hwnd, hdc);
                }
                else if (cmd == CLIP_LINE_SQUARE || cmd == CLIP_POINT_SQUARE || cmd == CLIP_POINT_SQUARE){
                    HDC hdc = GetDC(hwnd);
                    DrawSquare(hdc, 200, 200 , 300);
                    ReleaseDC(hwnd, hdc);
                }
            }
            break;
        }

        case WM_LBUTTONDOWN: {
            if (currShape == SPLINE_CARDINAL) {
                Point p;
                p.x = LOWORD(lParam);
                p.y = HIWORD(lParam);
                
                HDC hdc = GetDC(hwnd);
                DrawPoint(hdc, p.x, p.y, currColor);
                
                if (splinePointCount == 0) {
                    // First point, create new shape
                    Shape s;
                    s.algorithm = SPLINE_CARDINAL;
                    s.color = currColor;
                    s.tension = CARDINAL_C;
                    s.start = p;
                    s.splinePoints.push_back(p);
                    shapes.push_back(s);
                } else {
                    // Add point to existing shape
                    shapes.back().splinePoints.push_back(p);
                    
                    // Draw spline if we have enough points
                    if (shapes.back().splinePoints.size() >= 3) {
                        Vector2* points = new Vector2[shapes.back().splinePoints.size()];
                        for (size_t i = 0; i < shapes.back().splinePoints.size(); i++) {
                            points[i].x = shapes.back().splinePoints[i].x;
                            points[i].y = shapes.back().splinePoints[i].y;
                        }
                        DrawCardinalSpline(hdc, points, shapes.back().splinePoints.size(), CARDINAL_C, 100, currColor);
                        delete[] points;
                    }
                }
                
                splinePointCount++;
                ReleaseDC(hwnd, hdc);
            }
            else if (currShape == FILL_BEZIER) {
                startPoint.x = LOWORD(lParam);
                startPoint.y = HIWORD(lParam);
                isDrawing = true;
            }
            else {
                startPoint.x = LOWORD(lParam);
                startPoint.y = HIWORD(lParam);
            
                if (currShape == FILL_CONVEX || currShape == FILL_NONCONVEX) {
                    HDC hdc = GetDC(hwnd);
                    DrawPoint(hdc, startPoint.x, startPoint.y, currColor);
                    ReleaseDC(hwnd, hdc);
                }
                else if (currShape == CLIP_POLYGON_RECT) {
                    HDC hdc = GetDC(hwnd);
                    DrawPoint(hdc, startPoint.x, startPoint.y, RGB(0, 0, 0));
                    p_index++;
                    ReleaseDC(hwnd, hdc);
                }

                isDrawing = true;
            }
            break;
        }

        case WM_RBUTTONDOWN: {
            if (currShape == SPLINE_CARDINAL) {
                // Right click resets the spline
                splinePointCount = 0;
                InvalidateRect(hwnd, NULL, TRUE);
                return 0;
            }
            break;
        }

        case WM_LBUTTONUP: {
            if (!isDrawing) break;
            int x2 = LOWORD(lParam);
            int y2 = HIWORD(lParam);
            HDC hdc = GetDC(hwnd);

            Point endPoint;
            endPoint.x = x2;
            endPoint.y = y2;
            
            switch (currShape) {
                case LINE_DDA: {
                    DrawLineDDA(hdc, startPoint.x, startPoint.y, x2, y2, currColor);
                    Shape s = { startPoint, endPoint, currColor, currShape };
                    shapes.push_back(s);
                    break;
                }
                case LINE_MIDPOINT: {
                    DrawLineMidpoint(hdc, startPoint.x, startPoint.y, x2, y2, currColor);
                    Shape s = { startPoint, endPoint, currColor, currShape };
                    shapes.push_back(s);
                    break;
                }
                case LINE_PARAMETRIC: {
                    DrawLineParametric(hdc, startPoint.x, startPoint.y, x2, y2, currColor);
                    Shape s = { startPoint, endPoint, currColor, currShape };
                    shapes.push_back(s);
                    break;
                }
                case CIRCLE_DIRECT: {
                    int R = (int)sqrt((x2 - startPoint.x) * (x2 - startPoint.x) + 
                                   (y2 - startPoint.y) * (y2 - startPoint.y));
                    DrawCircleDirect(hdc, startPoint.x, startPoint.y, R, currColor);
                    Shape s = { startPoint, endPoint, currColor, currShape };
                    shapes.push_back(s);
                    break;
                }
                case CIRCLE_POLAR: {
                    int R = (int)sqrt((x2 - startPoint.x) * (x2 - startPoint.x) + 
                                   (y2 - startPoint.y) * (y2 - startPoint.y));
                    DrawCirclePolar(hdc, startPoint.x, startPoint.y, R, currColor);
                    Shape s = { startPoint, endPoint, currColor, currShape };
                    shapes.push_back(s);
                    break;
                }
                case CIRCLE_ITERATIVE_POLAR: {
                    int R = (int)sqrt((x2 - startPoint.x) * (x2 - startPoint.x) + 
                                   (y2 - startPoint.y) * (y2 - startPoint.y));
                    DrawCircleIterativePolar(hdc, startPoint.x, startPoint.y, R, currColor);
                    Shape s = { startPoint, endPoint, currColor, currShape };
                    shapes.push_back(s);
                    break;
                }
                case CIRCLE_MIDPOINT: {
                    int R = (int)sqrt((x2 - startPoint.x) * (x2 - startPoint.x) + 
                                   (y2 - startPoint.y) * (y2 - startPoint.y));
                    DrawCircleMidpoint(hdc, startPoint.x, startPoint.y, R, currColor);
                    Shape s = { startPoint, endPoint, currColor, currShape };
                    shapes.push_back(s);
                    break;
                }
                case CIRCEL_MODIFIED_MIDPOINT: {
                    int R = (int)sqrt((x2 - startPoint.x) * (x2 - startPoint.x) + 
                                   (y2 - startPoint.y) * (y2 - startPoint.y));
                    DrawCircleModifiedMidpoint(hdc, startPoint.x, startPoint.y, R, currColor);
                    Shape s = { startPoint, endPoint, currColor, currShape };
                    shapes.push_back(s);
                    break;
                }
                case FILL_CIRCLE_WITH_LINE: {
                    if (!circleisDrawn) {
                        // First click - draw the circle
                        int R = (int)sqrt((x2 - startPoint.x) * (x2 - startPoint.x) + 
                                      (y2 - startPoint.y) * (y2 - startPoint.y));
                        Shape s = { startPoint, endPoint, currColor, currShape };
                        DrawCircleMidpoint(hdc, startPoint.x, startPoint.y, R, currColor);
                        circleisDrawn = true;
                        cir_r_lines = R;
                        cir_x_line = startPoint.x;
                        cir_y_line = startPoint.y;
                        shapes.push_back(s);
                    } else {
                        // Create a new shape for this quarter fill
                        Shape s = shapes.back();  // Copy the last circle's properties
                        
                        // Set the appropriate quarter with current color
                        if (x2 >= cir_x_line && y2 <= cir_y_line) {
                            s.quartersLines[0] = make_pair(1, currColor);  // Top Right
                            FillCircleWithLines(hdc, cir_x_line, cir_y_line, 
                                              cir_r_lines, 1, currColor);
                            shapes.push_back(s);  // Save this quarter fill
                        }
                        else if (x2 < cir_x_line && y2 <= cir_y_line) {
                            s.quartersLines[1] = make_pair(1, currColor);  // Top Left
                            FillCircleWithLines(hdc, cir_x_line, cir_y_line, 
                                              cir_r_lines, 2, currColor);
                            shapes.push_back(s);  // Save this quarter fill
                        }
                        else if (x2 < cir_x_line && y2 > cir_y_line) {
                            s.quartersLines[2] = make_pair(1, currColor);  // Bottom Left
                            FillCircleWithLines(hdc, cir_x_line, cir_y_line, 
                                              cir_r_lines, 3, currColor);
                            shapes.push_back(s);  // Save this quarter fill
                        }
                        else if (x2 >= cir_x_line && y2 > cir_y_line) {
                            s.quartersLines[3] = make_pair(1, currColor);  // Bottom Right
                            FillCircleWithLines(hdc, cir_x_line, cir_y_line, 
                                              cir_r_lines, 4, currColor);
                            shapes.push_back(s);  // Save this quarter fill
                        }
                    }
                    break;
                }
                case FILL_CIRCLE_WITH_CIRCLE: {
                    if (!circleisDrawnWithCircle) {
                        // First click - draw the circle
                        int R = (int)sqrt((x2 - startPoint.x) * (x2 - startPoint.x) + 
                                      (y2 - startPoint.y) * (y2 - startPoint.y));
                        Shape s = { startPoint, endPoint, currColor, currShape };
                        DrawCircleMidpoint(hdc, startPoint.x, startPoint.y, R, currColor);
                        circleisDrawnWithCircle = true;
                        cir_r_with_circle = R;
                        cir_x_with_circle = startPoint.x;
                        cir_y_with_circle = startPoint.y;
                        shapes.push_back(s);
                    } else {
                        // Create a new shape for this quarter fill
                        Shape s = shapes.back();  // Copy the last circle's properties
                        
                        // Set the appropriate quarter with current color
                        if (x2 >= cir_x_with_circle && y2 <= cir_y_with_circle) {
                            s.quartersCircles[0] = make_pair(1, currColor);  // Top Right
                            FillCircleWithCircles(hdc, cir_x_with_circle, cir_y_with_circle, 
                                                cir_r_with_circle, 1, currColor);
                            shapes.push_back(s);  // Save this quarter fill
                        }
                        else if (x2 < cir_x_with_circle && y2 <= cir_y_with_circle) {
                            s.quartersCircles[1] = make_pair(1, currColor);  // Top Left
                            FillCircleWithCircles(hdc, cir_x_with_circle, cir_y_with_circle, 
                                                cir_r_with_circle, 2, currColor);
                            shapes.push_back(s);  // Save this quarter fill
                        }
                        else if (x2 < cir_x_with_circle && y2 > cir_y_with_circle) {
                            s.quartersCircles[2] = make_pair(1, currColor);  // Bottom Left
                            FillCircleWithCircles(hdc, cir_x_with_circle, cir_y_with_circle, 
                                                cir_r_with_circle, 3, currColor);
                            shapes.push_back(s);  // Save this quarter fill
                        }
                        else if (x2 >= cir_x_with_circle && y2 > cir_y_with_circle) {
                            s.quartersCircles[3] = make_pair(1, currColor);  // Bottom Right
                            FillCircleWithCircles(hdc, cir_x_with_circle, cir_y_with_circle, 
                                                cir_r_with_circle, 4, currColor);
                            shapes.push_back(s);  // Save this quarter fill
                        }
                    }
                    break;
                }
                case FILL_HERMITE: {
                    // Calculate initial dimensions
                    int left = min(startPoint.x, x2);
                    int right = max(startPoint.x, x2);
                    int top = min(startPoint.y, y2);
                    int bottom = max(startPoint.y, y2);
                    
                    // Make it a perfect square by using the larger dimension
                    int width = right - left;
                    int height = bottom - top;
                    int size = max(width, height);
                    
                    // Adjust dimensions to make it a square
                    if (width < height) {
                        // Expand width to match height
                        int expansion = (height - width) / 2;
                        left -= expansion;
                        right = left + size;
                    } else if (height < width) {
                        // Expand height to match width
                        int expansion = (width - height) / 2;
                        top -= expansion;
                        bottom = top + size;
                    }
                    
                    // Draw the square outline
                    HPEN pen = CreatePen(PS_SOLID, 1, currColor);
                    HPEN oldPen = (HPEN)SelectObject(hdc, pen);
                    MoveToEx(hdc, left, top, NULL);
                    LineTo(hdc, right, top);
                    LineTo(hdc, right, bottom);
                    LineTo(hdc, left, bottom);
                    LineTo(hdc, left, top);
                    SelectObject(hdc, oldPen);
                    DeleteObject(pen);
                    
                    // Fill the square with Hermite curves
                    FillSquareWithHermite(hdc, left, top, right, bottom, currColor);
                    Shape s = { startPoint, endPoint, currColor, currShape };
                    shapes.push_back(s);
                    break;
                }
                case FILL_BEZIER: {
                    // Calculate rectangle dimensions from the two points
                    int left = min(startPoint.x, x2);
                    int right = max(startPoint.x, x2);
                    int top = min(startPoint.y, y2);
                    int bottom = max(startPoint.y, y2);
                    
                    // Draw the rectangle outline
                    HPEN pen = CreatePen(PS_SOLID, 1, currColor);
                    HPEN oldPen = (HPEN)SelectObject(hdc, pen);
                    MoveToEx(hdc, left, top, NULL);
                    LineTo(hdc, right, top);
                    LineTo(hdc, right, bottom);
                    LineTo(hdc, left, bottom);
                    LineTo(hdc, left, top);
                    SelectObject(hdc, oldPen);
                    DeleteObject(pen);
                    
                    // Fill the rectangle with Bezier curves
                    FillRectangleWithBezier(hdc, left, top, right, bottom, currColor);
                    Shape s = { startPoint, endPoint, currColor, currShape };
                    shapes.push_back(s);
                    break;
                }
                case FILL_CONVEX: {
                    Shape s = { startPoint, endPoint, currColor, currShape };
                    pointsConvex[p_index].x = x2;
                    pointsConvex[p_index].y = y2;
                    p_index++;
                    if (p_index == 5) {
                        for (int i = 0; i < p_index; i++) {
                            s.extraPoints.push_back(pointsConvex[i]);
                        }
                        ConvexFilling(hdc, pointsConvex, p_index, currColor);
                        shapes.push_back(s);
                        p_index = 0;
                    }
                    break;
                }
                case FILL_NONCONVEX: {
                    Shape s = { startPoint, endPoint, currColor, currShape };
                    points[p_index].x = x2;
                    points[p_index].y = y2;
                    p_index++;
                    if (p_index == 5) {
                        for (int i = 0; i < p_index; i++) {
                            Point p;
                            p.x = points[i].x;
                            p.y = points[i].y;
                            s.extraPoints.push_back(p);
                        }
                        NonConvexFilling(hdc, points, p_index, currColor);
                        shapes.push_back(s);
                        p_index = 0;
                    }
                    break;
                }
                case FLOOD_FILL_RECURSIVE: {
                    Shape s = { startPoint, endPoint, currColor, currShape };
                    s.targetColorRecursive = GetPixel(hdc,startPoint.x,startPoint.y);
                    RecursiveFloodFill(hdc,startPoint.x,startPoint.y,s.targetColorRecursive,currColor);
                    shapes.push_back(s);
                    break;
                }
                case FLOOD_FILL_NONRECURSIVE: {
                    Shape s = { startPoint, endPoint, currColor, currShape };
                    s.targetColorNonRecursive = GetPixel(hdc, startPoint.x, startPoint.y);
                    NonRecursiveFloodFill(hdc, startPoint.x, startPoint.y, s.targetColorNonRecursive, currColor);
                    shapes.push_back(s);
                    break;
                }
                case ELLIPSE_DIRECT: {
                    DrawEllipseDirect(hdc, startPoint.x, startPoint.y, x2, y2, currColor);
                    Shape s = { startPoint, endPoint, currColor, currShape };
                    shapes.push_back(s);
                    break;
                }
                case ELLIPSE_POLAR: {
                    int xc = (startPoint.x + x2) / 2;
                    int yc = (startPoint.y + y2) / 2;
                    int a = abs(x2 - startPoint.x) / 2;  // horizontal radius
                    int b = abs(y2 - startPoint.y) / 2;  // vertical radius
                    DrawEllipsePolar(hdc, xc, yc, a, b, currColor);
                    Shape s = { startPoint, endPoint, currColor, currShape };
                    shapes.push_back(s);
                    break;
                }
                case ELLIPSE_MIDPOINT: {
                    int xc = (startPoint.x + x2) / 2;
                    int yc = (startPoint.y + y2) / 2;
                    int a = abs(x2 - startPoint.x) / 2;  // horizontal radius
                    int b = abs(y2 - startPoint.y) / 2;  // vertical radius
                    DrawEllipseMidpoint(hdc, xc, yc, a, b, currColor);
                    Shape s = { startPoint, endPoint, currColor, currShape };
                    shapes.push_back(s);
                    break;
                }
                case CLIP_LINE_RECT: {
                    ClipLineRectangle(hdc, startPoint.x, startPoint.y, x2, y2, 200, 100, 800, 300);
                    Shape s = { startPoint, endPoint, currColor, currShape };
                    shapes.push_back(s);
                    break;
                }
                case CLIP_LINE_SQUARE: {
                    ClipLineSquare(hdc, startPoint.x, startPoint.y, x2, y2, 200, 200, 500, 500);
                    Shape s = { startPoint, endPoint, currColor, currShape };
                    shapes.push_back(s);
                    break;
                }
                case CLIP_POINT_SQUARE: {
                    ClipPointSquare(hdc, startPoint.x, startPoint.y, 200, 200, 500, 500, currColor);
                    Shape s = { startPoint, endPoint, currColor, currShape };
                    shapes.push_back(s);
                    break;
                }
                case CLIP_POINT_RECT: {
                    ClipPointRectangle(hdc, startPoint.x, startPoint.y, 200, 100, 800, 300, currColor);
                    Shape s = { startPoint, endPoint, currColor, currShape };
                    shapes.push_back(s);
                    break;
                }
                case CLIP_POLYGON_RECT: {
                    Shape s = { startPoint, endPoint, currColor, currShape };
                    pointsPolygonClipping[p_index].x = (double)x2;
                    pointsPolygonClipping[p_index].y = (double)y2;
                    if(p_index == 6) {
                        for (int i = 0; i < p_index; i++) {
                            Point p;
                            p.x = pointsPolygonClipping[i].x;
                            p.y = pointsPolygonClipping[i].y;
                            s.extraPoints.push_back(p);
                        }
                        ClipPolygonRectangle(hdc, pointsPolygonClipping, p_index, 200, 100, 800, 300);
                        shapes.push_back(s);
                        p_index = 0;
                    }
                    break;
                }
                default: {
                    Shape s = { startPoint, endPoint, currColor, currShape };
                    shapes.push_back(s);
                    break;
                }
            }
            ReleaseDC(hwnd, hdc);
            isDrawing = false;
            break;
        }

        case WM_DESTROY:
            PostQuitMessage(0);
            break;

        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}



// ------------------------------------- End of WindowProc ------------------------------------//
// ------------------------------------- End of WindowProc ------------------------------------//
// ------------------------------------- End of WindowProc ------------------------------------//
// ------------------------------------- End of WindowProc ------------------------------------//


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow) {
    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = "2D Drawing App";
    wc.hCursor = LoadCursor(NULL, IDC_HAND);
    wc.hbrBackground = (HBRUSH) (CreateSolidBrush(RGB(255, 255, 255)));

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        0, "2D Drawing App", "Drawing Algorithms", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 900, 700,
        NULL, NULL, hInstance, NULL
    );

    if (!hwnd)
        return 0;
    ShowWindow(hwnd, nCmdShow);

    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}
    
