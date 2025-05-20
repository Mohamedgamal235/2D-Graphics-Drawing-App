#include <stack>
#include <windows.h>

// Osama
void RecursiveFloodFill(HDC hdc ,int x, int y, COLORREF oldColor, COLORREF newColor){

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