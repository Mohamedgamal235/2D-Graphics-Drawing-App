#include <windows.h>
using namespace std;


void drawDot(HDC hdc, int x, int y, COLORREF color, int size = 5) {
    int half = size / 2;
    for (int dx = -half; dx <= half; ++dx) {
        for (int dy = -half; dy <= half; ++dy) {
            SetPixel(hdc, x + dx, y + dy, color);
        }
    }
}

// Anas Adel
void ClipPointRectangle(HDC hdc) {
}


// -------------------------------------------


// Ahmed Mohsen
void ClipPointSquare(HDC hdc, int x, int y, int top, int left, int bottom, int right, COLORREF c, int r) {
    if (x < right && x > left && y < bottom && y > top) {
        drawDot(hdc, x, y, c, r);
    }
}
