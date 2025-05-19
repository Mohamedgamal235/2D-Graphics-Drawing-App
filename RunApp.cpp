#include <windows.h>
#include <string>
#include <cmath>

// Menu identifiers
#define ID_CIRCLE_DIRECT          101
#define ID_CIRCLE_POLAR           102
#define ID_CIRCLE_ITERATIVE       103
#define ID_CIRCLE_MIDPOINT        104
#define ID_CIRCLE_MODIFIED        105

// Global variables
HINSTANCE hInst;
int currentAlgorithm = ID_CIRCLE_DIRECT;

// Function prototypes
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void DrawCircleDirect(HDC hdc, int xc, int yc, int r);
void DrawCirclePolar(HDC hdc, int xc, int yc, int r);
void DrawCircleIterative(HDC hdc, int xc, int yc, int r);
void DrawCircleMidpoint(HDC hdc, int xc, int yc, int r);
void DrawCircleModifiedMidpoint(HDC hdc, int xc, int yc, int r);

// Entry point
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow) {
    hInst = hInstance;

    // Register window class
    WNDCLASS wc = {};
    wc.lpfnWndProc   = WindowProc;
    wc.hInstance     = hInstance;
    wc.lpszClassName = "Win32CircleApp";
    wc.hCursor       = LoadCursor(NULL, IDC_CROSS);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

    RegisterClass(&wc);

    // Create window
    HWND hwnd = CreateWindowEx(
        0,
        "Win32CircleApp",
        "Circle Drawing ",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 800, 600,
        NULL, NULL, hInstance, NULL
    );

    if (!hwnd) return 0;

    ShowWindow(hwnd, nCmdShow);

    // Message loop
    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

// Window procedure
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    static POINT clickPoint = {0, 0};

    switch (uMsg) {
        case WM_CREATE: {
            // Create menu
            HMENU hMenu = CreateMenu();
            HMENU hCircleMenu = CreatePopupMenu();
            AppendMenu(hCircleMenu, MF_STRING, ID_CIRCLE_DIRECT, "Direct");
            AppendMenu(hCircleMenu, MF_STRING, ID_CIRCLE_POLAR, "Polar");
            AppendMenu(hCircleMenu, MF_STRING, ID_CIRCLE_ITERATIVE, "Iterative Polar");
            AppendMenu(hCircleMenu, MF_STRING, ID_CIRCLE_MIDPOINT, "Midpoint");
            AppendMenu(hCircleMenu, MF_STRING, ID_CIRCLE_MODIFIED, "Modified Midpoint");
            AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hCircleMenu, "Circle Algorithms");
            SetMenu(hwnd, hMenu);
            break;
        }
        case WM_COMMAND: {
            switch (LOWORD(wParam)) {
                case ID_CIRCLE_DIRECT:
                case ID_CIRCLE_POLAR:
                case ID_CIRCLE_ITERATIVE:
                case ID_CIRCLE_MIDPOINT:
                case ID_CIRCLE_MODIFIED:
                    currentAlgorithm = LOWORD(wParam);
                    break;
            }
            break;
        }
        case WM_LBUTTONDOWN: {
            clickPoint.x = LOWORD(lParam);
            clickPoint.y = HIWORD(lParam);
            break;
        }
        case WM_LBUTTONUP: {
            int x = LOWORD(lParam);
            int y = HIWORD(lParam);
            int dx = x - clickPoint.x;
            int dy = y - clickPoint.y;
            int radius = static_cast<int>(sqrt(dx * dx + dy * dy));

            HDC hdc = GetDC(hwnd);
            switch (currentAlgorithm) {
                case ID_CIRCLE_DIRECT:
                    DrawCircleDirect(hdc, clickPoint.x, clickPoint.y, radius);
                    break;
                case ID_CIRCLE_POLAR:
                    DrawCirclePolar(hdc, clickPoint.x, clickPoint.y, radius);
                    break;
                case ID_CIRCLE_ITERATIVE:
                    DrawCircleIterative(hdc, clickPoint.x, clickPoint.y, radius);
                    break;
                case ID_CIRCLE_MIDPOINT:
                    DrawCircleMidpoint(hdc, clickPoint.x, clickPoint.y, radius);
                    break;
                case ID_CIRCLE_MODIFIED:
                    DrawCircleModifiedMidpoint(hdc, clickPoint.x, clickPoint.y, radius);
                    break;
            }
            ReleaseDC(hwnd, hdc);
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

// Drawing functions
void DrawCircleDirect(HDC hdc, int xc, int yc, int r) {
    for (int x = 0; x <= r / sqrt(2); ++x) {
        int y = static_cast<int>(sqrt(r * r - x * x));
        SetPixel(hdc, xc + x, yc + y, RGB(0, 0, 0));
        SetPixel(hdc, xc - x, yc + y, RGB(0, 0, 0));
        SetPixel(hdc, xc + x, yc - y, RGB(0, 0, 0));
        SetPixel(hdc, xc - x, yc - y, RGB(0, 0, 0));
        SetPixel(hdc, xc + y, yc + x, RGB(0, 0, 0));
        SetPixel(hdc, xc - y, yc + x, RGB(0, 0, 0));
        SetPixel(hdc, xc + y, yc - x, RGB(0, 0, 0));
        SetPixel(hdc, xc - y, yc - x, RGB(0, 0, 0));
    }
}

void DrawCirclePolar(HDC hdc, int xc, int yc, int r) {
    // Placeholder for Polar algorithm implementation
}

void DrawCircleIterative(HDC hdc, int xc, int yc, int r) {
    // Placeholder for Iterative Polar algorithm implementation
}

void DrawCircleMidpoint(HDC hdc, int xc, int yc, int r) {
    // Placeholder for Midpoint algorithm implementation
}

void DrawCircleModifiedMidpoint(HDC hdc, int xc, int yc, int r) {
    // Placeholder for Modified Midpoint algorithm implementation
}
