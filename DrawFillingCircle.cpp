#include "DrawFilling.h"
#include "DrawLine.h"
#include "DrawCircle.h"
#include <cmath>

void FillCircleWithLines(HDC hdc, int xc, int yc, int r, int quarter, COLORREF color) {
    // Quarter 1: Top Right
    // Quarter 2: Top Left
    // Quarter 3: Bottom Left
    // Quarter 4: Bottom Right

    switch(quarter) {
        case 1: // Top Right
            for(int x = xc; x <= xc + r; x++) {
                int y1 = yc;
                int y2 = yc - (int)sqrt(r * r - (x - xc) * (x - xc));
                DrawLineMidpoint(hdc , x , y1 , x , y2 , color);
            }
            break;
            
        case 2: // Top Left
            for(int x = xc; x >= xc - r; x--) {
                int y1 = yc;
                int y2 = yc - (int)sqrt(r * r - (x - xc) * (x - xc));
                DrawLineMidpoint(hdc , x , y1 , x , y2 , color);
            }
            break;
            
        case 3: // Bottom Left
            for(int x = xc; x >= xc - r; x--) {
                int y1 = yc;
                int y2 = yc + (int)sqrt(r * r - (x - xc) * (x - xc));
                DrawLineMidpoint(hdc , x , y1 , x , y2 , color);
            }
            break;
            
        case 4: // Bottom Right
            for(int x = xc; x <= xc + r; x++) {
                int y1 = yc;
                int y2 = yc + (int)sqrt(r * r - (x - xc) * (x - xc));
                DrawLineMidpoint(hdc , x , y1 , x , y2 , color);
            }
            break;
    }
}