#include <windows.h>
#include <cmath>
using namespace std ;
#define ll long long
#define M_PI 3.14159265358979323846

void Draw4Points(HDC hdc, int xc, int yc, int x, int y, COLORREF color) {
    SetPixel(hdc, xc + x, yc + y, color);
    SetPixel(hdc, xc - x, yc + y, color);
    SetPixel(hdc, xc + x, yc - y, color);
    SetPixel(hdc, xc - x, yc - y, color);
}


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
    for (t = 0; t < 2 * M_PI; t += 0.0001) {
        int x = (int) (a * cos(t));
        int y = (int) (b * sin(t));

        int xrot = (int) (x * cos(theta) - y * sin(theta));
        int yrot = (int) (x * sin(theta) + y * cos(theta));

        SetPixel(hdc, xc + xrot, yc + yrot, c);
    }
}

// -------------------------------------------

// Ahmed Mohsen
void DrawEllipsePolar(HDC hdc, int xc, int yc, int a, int b, COLORREF c) {
    double theta = 0;
    double dtheta = 1.0 / 1000;
    double x, y;

    while (theta < 2 * M_PI) {
        x = a * cos(theta);
        y = b * sin(theta);
        Draw4Points(hdc, xc, yc, round(x), round(y), c);
        theta += dtheta;
    }
}

// -------------------------------------------

// Mohamed Gamal
void DrawEllipseMidpoint(HDC hdc, int xc, int yc, int a, int b, COLORREF color) {
    int x = 0;
    int y = b;

    ll a2 = (ll) a * a;
    ll b2 = (ll) b * b;

    ll d1 = b2 - a2 * b + (a2 / 4);

    ll dx = 2 * b2 * x;
    ll dy = 2 * a2 * y;

    while (dx < dy) {
        // slope < 1
        Draw4Points(hdc, xc, yc, x, y, color);
        if (d1 < 0)
            d1 += b2 * (2 * x + 3);
        else {
            d1 += (b2 * (2 * x + 3) - a2 * (2 * y - 2));
            y--;
            dy -= 2 * a2;
        }
        x++;
        dx += 2 * b2;
    }


    ll d2 = b2 * (x + 0.5) * (x + 0.5) + a2 * (y - 1) * (y - 1) - a2 * b2;
    while (y >= 0) {
        Draw4Points(hdc, xc, yc, x, y, color);
        if (d2 > 0)
            d2 += a2 * (3 - 2 * y);
        else {
            x++;
            dx += 2 * b2;
            d2 += b2 * (2 * x + 2) + a2 * (3 - 2 * y);
        }
        y--;
        dy -= 2 * a2;
    }
}
