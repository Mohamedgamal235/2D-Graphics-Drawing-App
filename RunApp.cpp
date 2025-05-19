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
    NONE ,
    LINE_DDA ,
    LINE_MIDPOINT ,
    LINE_PARAMETRIC,
    CIRCLE_DIRECT ,
    CIRCLE_POLAR,
    CIRCLE_ITERATIVE_POLAR,
    CIRCLE_MIDPOINT,
    CIRCEL_MODIFIED_MIDPOINT,
    FILL_CIRCLE_WITH_LINE ,
    FILL_CIRCLE_WITH_CIRCLE ,
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
    RED ,
    GREEN,
    BLUE ,
    BLACK ,
    CLEAR_SCREEN ,
    SAVE_SCREEN ,
    LOAD_SCREEN
};

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow) {
    WNDCLASS wc = {};
    wc.lpfnWndProc   = WindowProc;
    wc.hInstance     = hInstance;
    wc.lpszClassName = "2D Drawing App";
    wc.hCursor       = LoadCursor(NULL, IDC_HAND);
    wc.hbrBackground = (HBRUSH)(CreateSolidBrush(RGB(255,255,255)));

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        0, "2D Drawing AppGraphicsAlgorithms", "Drawing Algorithms", WS_OVERLAPPEDWINDOW,
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