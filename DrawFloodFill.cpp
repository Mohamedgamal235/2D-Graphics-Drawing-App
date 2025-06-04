#include <stack>
#include <windows.h>
#include "DrawFloodFill.h"
using namespace std;

struct Point {
    int x, y;
    Point(int x = 0, int y = 0) : x(x), y(y) {}
};

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


void NonRecursiveFloodFill(HDC hdc, int x, int y, COLORREF bc, COLORREF fc) {
    if (bc == fc) 
        return;  
    
    stack<Point> st;
    st.push(Point(x, y));

    while (!st.empty()) {
        Point current = st.top();
        st.pop();
        
        COLORREF currColor = GetPixel(hdc, current.x, current.y);
        
        if (currColor != bc) continue;
        
        SetPixel(hdc, current.x, current.y, fc);
        st.push(Point(current.x + 1, current.y));
        st.push(Point(current.x - 1, current.y));
        st.push(Point(current.x, current.y + 1));
        st.push(Point(current.x, current.y - 1));
    }
}