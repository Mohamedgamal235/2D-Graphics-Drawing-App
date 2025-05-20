#include <windows.h>

// Anas Adel
void DrawEllipseDirect(HDC hdc, int x1, int y1, int x2, int y2, COLORREF c) {
    int xc = (x1 + x2) / 2;
    int yc = (y1 + y2) / 2;

    double dx = x2 - x1;
    double dy = y2 - y1;
    double a = sqrt(dx * dx + dy * dy) / 2;

    double b = a / 2;

    double theta = atan2(dy, dx);

    double t;
    for (t = 0; t < 2 * M_PI; t += 0.001) {
        int x = (int)(a * cos(t));
        int y = (int)(b * sin(t));

        int xrot = (int)(x * cos(theta) - y * sin(theta));
        int yrot = (int)(x * sin(theta) + y * cos(theta));

        SetPixel(hdc, xc + xrot, yc + yrot, c);
    }
}

// -------------------------------------------

// Ahmed Mohsen
void DrawEllipsePolar(HDC hdc){

}

// -------------------------------------------

// Mohamed Gamal
void DrawEllipseMidpoint(HDC hdc){

}
