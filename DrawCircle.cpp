#include <windows.h>

// Anas Adel
void DrawCircleDirect(HDC hdc, int xc, int yc, int r){

}

// -------------------------------------------

// Osama
void DrawCirclePolar(HDC hdc, int xc, int yc, int r){

}

// -------------------------------------------

// Mohamed Gamal
void DrawCircleIterativePolar(HDC hdc, int xc, int yc, int r){

}

// -------------------------------------------

// Anas Adel
void DrawCircleMidpoint(HDC hdc, int xc, int yc, int r){
    int x = 0;
    int y = R;
    int d = 1 - R;
    while (x <= y) {
        SetPixel(hdc, xc + x, yc + y, RGB(0, 0, 0));
        SetPixel(hdc, xc - x, yc + y, RGB(0, 0, 0));
        SetPixel(hdc, xc + x, yc - y, RGB(0, 0, 0));
        SetPixel(hdc, xc - x, yc - y, RGB(0, 0, 0));
        SetPixel(hdc, xc + y, yc + x, RGB(0, 0, 0));
        SetPixel(hdc, xc - y, yc + x, RGB(0, 0, 0));
        SetPixel(hdc, xc + y, yc - x, RGB(0, 0, 0));
        SetPixel(hdc, xc - y, yc - x, RGB(0, 0, 0));
    
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
