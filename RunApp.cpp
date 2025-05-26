#include <windows.h>
#include "DrawCircle.cpp"
#include "DrawClipping.cpp"
#include "DrawEllips.cpp"
#include "DrawFilling.cpp"
#include "DrawFloodFill.cpp"
#include "DrawLine.cpp"
#include "DrawSpline.cpp"
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

struct Point {
    int x, y;
};

Point startPoint;
DrawShap currShape = NONE;
COLORREF currColor = RGB(0, 0, 0); // black
bool isDrawing = false;

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

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_CREATE:
            creatDrawingMenu(hwnd);
            break;

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
            }
            // else if (cmd == SAVE_SCREEN) {
            //
            // }
            // else if (cmd == LOAD_SCREEN) {
            //
            // }
            else {
                currShape = cmd;
            }
            break;
        }

        case WM_LBUTTONDOWN:
            // ممكن نعمل هنا سويتش بس لسه بفكر فيها تتعمل ازاي ممكن تكتب كود عادي
            // وابقي غير الحته دي اول ما يعمل رن معاك رجعها مكانها بعد اذنك
            startPoint.x = LOWORD(lParam);
            startPoint.y = HIWORD(lParam);
            isDrawing = true;
            break;

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
                case CIRCEL_MODIFIED_MIDPOINT: {
                    int R = (int) sqrt(
                        (x2 - startPoint.x) * (x2 - startPoint.x) + (y2 - startPoint.y) * (y2 - startPoint.y));

                    DrawCircleModifiedMidpoint(hdc, startPoint.x, startPoint.y, R, currColor);
                    break;
                }

                // case CIRCLE_ITERATIVE_POLAR:
                //     // code
                //     break;
                case CIRCLE_MIDPOINT: {
                    int R = (int) sqrt(
                        (x2 - startPoint.x) * (x2 - startPoint.x) + (y2 - startPoint.y) * (y2 - startPoint.y));
                    DrawCircleMidpoint(hdc, startPoint.x, startPoint.y, R, currColor);
                    break;
                }
                // case CIRCEL_MODIFIED_MIDPOINT:
                //     // code
                //     break;
                // case FILL_CIRCLE_WITH_LINE:
                //     // code
                //     break;
                // case FILL_CIRCLE_WITH_CIRCLE:
                //     // code
                //     break;
                // case FILL_HERMITE:
                //     // code
                //     break;
                // case FILL_BEZIER:
                //     // code
                //     break;
                // case FILL_CONVEX:
                //     // code
                //     break;
                // case FILL_NONCONVEX:
                //     // code
                //     break;
                 case FLOOD_FILL_RECURSIVE: {
//                     COLORREF c = GetPixel(hdc,startPoint.x,startPoint.y);
//                     RecursiveFloodFill(hdc,startPoint.x,startPoint.y,c,currColor);
                     break;
                 }
                case FLOOD_FILL_NONRECURSIVE:
                    NonRecursiveFloodFill(hdc, startPoint.x, startPoint.y, currColor, currColor);
                    break;
                // case SPLINE_CARDINAL:
                //     // code
                //     break;
                // case ELLIPSE_DIRECT:
                //     // code
                //     break;
                // case ELLIPSE_POLAR:
                //     // code
                //     break;
                // case ELLIPSE_MIDPOINT:
                //     // code
                //     break;
                // case CLIP_POINT_RECT:
                //     // code
                //     break;
                // case CLIP_LINE_RECT:
                //     // code
                //     break;
                // case CLIP_POLYGON_RECT:
                //     // code
                //     break;
                // case CLIP_POINT_SQUARE:
                //     // code
                //     break;
                // case CLIP_LINE_SQUARE:
                //     // code
                //     break;
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
