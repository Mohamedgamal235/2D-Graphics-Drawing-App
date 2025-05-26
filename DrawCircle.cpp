#include <windows.h>
#include <cmath>

void Draw8Points(HDC hdc, int xc, int yc, int x, int y, COLORREF color) {
    SetPixel(hdc, xc + x, yc + y, color);
    SetPixel(hdc, xc - x, yc + y, color);
    SetPixel(hdc, xc + x, yc - y, color);
    SetPixel(hdc, xc - x, yc - y, color);
    SetPixel(hdc, xc + y, yc + x, color);
    SetPixel(hdc, xc - y, yc + x, color);
    SetPixel(hdc, xc + y, yc - x, color);
    SetPixel(hdc, xc - y, yc - x, color);
}


// Osama use Draw8Points above
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
void DrawCircleIterativePolar(HDC hdc, int xc, int yc, int r , COLORREF color){
    double x = r , y = 0 ; 
    double dtheta = 1.0 / r ; 
    double cdtheta = cos(dtheta);
    double sdtheta = sin(dtheta);
    Draw8Points(hdc, xc, yc, x, y, color);
    while (x > y){
        double x1 = x * cdtheta - y * sdtheta ; 
        double y1 = x * sdtheta + y * cdtheta ; 
        x = x1 ; 
        y = y1 ; 
        Draw8Points(hdc, xc, yc, round(x), round(y), color);
    }
}

// -------------------------------------------

// Anas Adel
void DrawCircleMidpoint(HDC hdc, int xc, int yc, int r, COLORREF color) {
    int x = 0;
    int y = r;
    int d = 1 - r ;
    while (x <= y) {
        Draw8Points(hdc, xc, yc, x, y, color);
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

// Ahmed Mohsen -> use Draw8Points above
void DrawCircleModifiedMidpoint(HDC hdc, int xc, int yc, int r ,COLORREF c) {
    int x = 0, y = r, d = 1 - r;
    int d1 = 3, d2 = 5 - 2 * r;
    Draw8Points(hdc, xc, yc, x, y, c);
    while (x < y) {
        if (d < 0) {
            d += d1;
            d1 += 2;
            d2 += 2;
            x++;
        } else {
            d += d2;
            d1 += 2;
            d2 += 4;
            x++;
            y--;
        }
        Draw8Points(hdc, xc, yc, x, y, c);
    }
}
