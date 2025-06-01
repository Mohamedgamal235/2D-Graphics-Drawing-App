#include <windows.h>
#include <cmath>
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



Point startPoint;
DrawShap currShape = NONE;
COLORREF currColor = RGB(0, 0, 0); // black
bool isDrawing = false;


bool circleisDrawn = false;
int cir_r = 0 ; 
int cir_x = 0 ;
int cir_y  = 0 ; 


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

PolygonPoint points[6];
int p_index = 0;
Point points2[5];
Vertex points3[6] = {Vertex(0,0), Vertex(0,0), Vertex(0,0), Vertex(0,0), Vertex(0,0), Vertex(0,0)};


void DrawPoint(HDC hdc, int x, int y, COLORREF color) {
    HBRUSH hBrush = CreateSolidBrush(color);
    HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
    Ellipse(hdc, x - 2, y - 2, x + 2, y + 2);
    SelectObject(hdc, oldBrush);
    DeleteObject(hBrush);
}


LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    static POINT startPt;
    static bool isDrawing = false;
    HDC hdc;
    PAINTSTRUCT ps;

    switch (uMsg) {
        case WM_CREATE:
            creatDrawingMenu(hwnd);
            break;

        case WM_PAINT: {
            hdc = BeginPaint(hwnd, &ps);
            
            if (currShape == FILL_CONVEX) {
                for (int i = 0; i < p_index; i++) {
                    DrawPoint(hdc, points2[i].x, points2[i].y, currColor);
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
                InvalidateRect(hwnd, NULL, TRUE);
                circleisDrawn = false;
            }
            else if (cmd == SAVE_SCREEN) {
            }
            else if (cmd == LOAD_SCREEN) {
            }
            else {
                currShape = cmd;
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
                // Reset states when selecting circle filling options
                if (cmd == FILL_CIRCLE_WITH_LINE ) {
                    circleisDrawn = false;
                }
            }
            break;
        }

        case WM_LBUTTONDOWN: {
            if (currShape == FILL_BEZIER) {
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

        case WM_LBUTTONUP: {
            if (!isDrawing) break;
            int x2 = LOWORD(lParam);
            int y2 = HIWORD(lParam);
            HDC hdc = GetDC(hwnd);

           

            switch (currShape) {
                case LINE_DDA:
                    DrawLineDDA(hdc, startPoint.x, startPoint.y, x2, y2, currColor);
                    break;
                case LINE_MIDPOINT:
                    DrawLineMidpoint(hdc, startPoint.x, startPoint.y, x2, y2, currColor);
                    break;
                case LINE_PARAMETRIC:
                    DrawLineParametric(hdc, startPoint.x, startPoint.y, x2, y2, currColor);
                    break;
                case CIRCLE_DIRECT: {
                    int R = (int) sqrt(
                            (x2 - startPoint.x) * (x2 - startPoint.x) + (y2 - startPoint.y) * (y2 - startPoint.y));
                    DrawCircleDirect(hdc,startPoint.x,startPoint.y,R,currColor);
                    break;
                }
                case CIRCLE_POLAR: {
                    int R = (int) sqrt(
                        (x2 - startPoint.x) * (x2 - startPoint.x) + (y2 - startPoint.y) * (y2 - startPoint.y));
                    DrawCirclePolar(hdc, startPoint.x, startPoint.y, R, currColor);
                    break;
                }
                case CIRCLE_ITERATIVE_POLAR:{
                    int R = (int) sqrt(
                        (x2 - startPoint.x) * (x2 - startPoint.x) + (y2 - startPoint.y) * (y2 - startPoint.y));
                    DrawCircleIterativePolar(hdc, startPoint.x, startPoint.y, R, currColor);
                    break;
                }
                case CIRCLE_MIDPOINT: {
                    int R = (int) sqrt(
                        (x2 - startPoint.x) * (x2 - startPoint.x) + (y2 - startPoint.y) * (y2 - startPoint.y));
                    DrawCircleMidpoint(hdc, startPoint.x, startPoint.y, R, currColor);
                    break;
                }
                case CIRCEL_MODIFIED_MIDPOINT: {
                    int R = (int) sqrt(
                        (x2 - startPoint.x) * (x2 - startPoint.x) + (y2 - startPoint.y) * (y2 - startPoint.y));
                    DrawCircleModifiedMidpoint(hdc, startPoint.x, startPoint.y, R, currColor);
                    break;
                }

                case FILL_CIRCLE_WITH_LINE: {
                    if (!circleisDrawn) {
                        // First click - draw the circle
                        cir_r = (int)sqrt((x2 - startPoint.x) * (x2 - startPoint.x) + 
                                       (y2 - startPoint.y) * (y2 - startPoint.y));
                        cir_x = startPoint.x;
                        cir_y = startPoint.y;
                        DrawCircleMidpoint(hdc, cir_x, cir_y, cir_r, currColor);
                        circleisDrawn = true;
                    }
                    else {
                        // Second click - determine quarter and fill
                        int quarter = 0;
                        if (x2 >= cir_x && y2 <= cir_y) 
                            quarter = 1;      // Top Right
                        else if (x2 < cir_x && y2 <= cir_y) 
                            quarter = 2;      // Top Left
                        else if (x2 < cir_x && y2 > cir_y) 
                            quarter = 3;      // Bottom Left
                        else if (x2 >= cir_x && y2 > cir_y) 
                            quarter = 4;      // Bottom Right
                        
                        if (quarter > 0) {
                            FillCircleWithLines(hdc, cir_x, cir_y, cir_r, quarter, currColor);
                        }
                    }
                    break;
                }

                case FILL_CIRCLE_WITH_CIRCLE: {
                    if (!circleisDrawn) {
                        // First click - draw the circle
                        cir_r = (int)sqrt((x2 - startPoint.x) * (x2 - startPoint.x) + 
                                       (y2 - startPoint.y) * (y2 - startPoint.y));
                        cir_x = startPoint.x;
                        cir_y = startPoint.y;
                        DrawCircleMidpoint(hdc, cir_x, cir_y, cir_r, currColor);
                        circleisDrawn = true;
                    }
                    else {
                        // Second click - determine quarter and fill
                        int quarter = 0;
                        if (x2 >= cir_x && y2 <= cir_y) 
                            quarter = 1;      // Top Right
                        else if (x2 < cir_x && y2 <= cir_y) 
                            quarter = 2;      // Top Left
                        else if (x2 < cir_x && y2 > cir_y) 
                            quarter = 3;      // Bottom Left
                        else if (x2 >= cir_x && y2 > cir_y) 
                            quarter = 4;      // Bottom Right
                        
                        if (quarter > 0) {
                            FillCircleWithCircles(hdc, cir_x, cir_y, cir_r, quarter, currColor);
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
                    break;
                }

                case FILL_CONVEX: {
                    points2[p_index].x = x2;
                    points2[p_index].y = y2;
                    p_index++;
                    if (p_index == 5) {
                        ConvexFilling(hdc, points2, p_index, currColor);
                        p_index = 0;
                    }
                    break;
                }
                case FILL_NONCONVEX: {
                    points[p_index].x = x2;
                    points[p_index].y = y2;
                    p_index++;
                    if (p_index == 6) {
                        NonConvexFilling(hdc, points, p_index, currColor);
                        p_index = 0;
                    }
                    break;
                }
                case FLOOD_FILL_RECURSIVE: {
                    COLORREF c = GetPixel(hdc,startPoint.x,startPoint.y);
                    RecursiveFloodFill(hdc,startPoint.x,startPoint.y,c,currColor);
                    break;
                }
                case FLOOD_FILL_NONRECURSIVE:
                    NonRecursiveFloodFill(hdc, startPoint.x, startPoint.y, currColor, currColor);
                    break;
                case ELLIPSE_DIRECT:
                    DrawEllipseDirect(hdc, startPoint.x, startPoint.y, x2, y2, currColor);
                    break;
                case ELLIPSE_POLAR:{
                        int xc = (startPoint.x + x2) / 2;
                        int yc = (startPoint.y + y2) / 2;
                        int a = abs(x2 - startPoint.x) / 2;  // horizontal radius
                        int b = abs(y2 - startPoint.y) / 2;  // vertical radius
                        DrawEllipsePolar(hdc, xc, yc, a, b, currColor);
                    }
                    break;
                case ELLIPSE_MIDPOINT: {
                        int xc = (startPoint.x + x2) / 2;
                        int yc = (startPoint.y + y2) / 2;
                        int a = abs(x2 - startPoint.x) / 2;  // horizontal radius
                        int b = abs(y2 - startPoint.y) / 2;  // vertical radius
                        DrawEllipseMidpoint(hdc, xc, yc, a, b, currColor);
                        break;
                    }
                case CLIP_LINE_RECT:
                    ClipLineRectangle(hdc, startPoint.x, startPoint.y, x2, y2, 200, 100, 800, 300);
                    break;

                case CLIP_LINE_SQUARE:
                    ClipLineSquare(hdc , startPoint.x , startPoint.y , x2 , y2 ,200 , 200 , 500 , 500);
                    break;

                case CLIP_POLYGON_RECT: {
                    points3[p_index].x = (double)x2 ;
                    points3[p_index].y = (double)y2 ;
                    if(p_index == 6) {
                        ClipPolygonRectangle(hdc, points3, p_index, 200, 100, 800, 300);
                        p_index = 0 ;
                    }
                }
                case CLIP_POINT_SQUARE: {
                    int pointRadius =5;
                    ClipPointSquare(hdc,startPoint.x,startPoint.y,200,200,500,500,currColor,pointRadius);
                }
                    break;

                default:
                    break;
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
    
