#include <windows.h>
#include <cmath>

// Osama
void DrawCircleDirect(HDC hdc, int xc, int yc, int r, COLORREF color) {

}

// -------------------------------------------

// Anas Adel
void DrawCirclePolar(HDC hdc, int xc, int yc, int r , COLORREF color){
    double theta;
    int x, y;

    double step = 1.0 / r;

    for (theta = 0; theta <= 2 * 3.14159; theta += step) {
        x = (int)(xc + r * cos(theta));
        y = (int)(yc + r * sin(theta));
        SetPixel(hdc, x, y, color);
    }
}

// -------------------------------------------

// Mohamed Gamal
void DrawCircleIterativePolar(HDC hdc, int xc, int yc, int r){

}

// -------------------------------------------

// Anas Adel
void DrawCircleMidpoint(HDC hdc, int xc, int yc, int r, COLORREF color) {
    int x = 0;
    int y = R;
    int d = 1 - R;
    while (x <= y) {
        SetPixel(hdc, xc + x, yc + y, color);
        SetPixel(hdc, xc - x, yc + y, color);
        SetPixel(hdc, xc + x, yc - y, color);
        SetPixel(hdc, xc - x, yc - y, color);
        SetPixel(hdc, xc + y, yc + x, color);
        SetPixel(hdc, xc - y, yc + x, color);
        SetPixel(hdc, xc + y, yc - x, color);
        SetPixel(hdc, xc - y, yc - x, color);

        if (d < 0) {
            x++;
            d += 2 * x + 3;
        }
        else {
            x++;
            y--;
            d += 2 * x - 2 * y + 5;
        }
    }
}

// -------------------------------------------

// Ahmed Mohsen
void DrawCircleModifiedMidpoint(HDC hdc, int xc, int yc, int r){

}
