#include <stack>
#include <windows.h>
#include "DrawFloodFill.h"

// Osama
void RecursiveFloodFill(HDC hdc ,int x, int y, COLORREF oldColor, COLORREF newColor){
    COLORREF c = GetPixel(hdc,x,y);
    if(c != oldColor || c == newColor ) return;
    SetPixel(hdc,x,y,newColor);
    RecursiveFloodFill(hdc,x+1,y,oldColor,newColor);
    RecursiveFloodFill(hdc,x,y+1,oldColor,newColor);
    RecursiveFloodFill(hdc,x-1,y,oldColor,newColor);
    RecursiveFloodFill(hdc,x,y-1,oldColor,newColor);
}

// -------------------------------------------

// Ahmed Mohsen
void NonRecursiveFloodFill(HDC hdc, int x, int y, COLORREF oldColor, COLORREF newColor ){

    COLORREF targetColor = GetPixel(hdc, x, y);
    if (targetColor == oldColor) return;

    std::stack<std::pair<int, int>> s;
    s.push({x, y});

    while (!s.empty()) {
        auto [cx, cy] = s.top();
        s.pop();

        COLORREF current = GetPixel(hdc, cx, cy);
        if (current != targetColor) continue;

        SetPixel(hdc, cx, cy, oldColor);

        s.push({cx + 1, cy});
        s.push({cx - 1, cy});
        s.push({cx, cy + 1});
        s.push({cx, cy - 1});
    }
}