#include <windows.h>
#include "DrawLine.h"
#include "DrawLineClipping.h"

// Utalitiy Functions
OutCode getOutCode(double x, double y,double x_left, double x_right, double y_top, double y_bottom){
    OutCode result;
    result.all = 0;
    if(x < x_left) result.left = 1; else if(x > x_right) result.right = 1;
    if(y > y_bottom) result.bottom = 1; else if (y < y_top) result.top = 1; // in window y_top < y_bottom
    return result;
}
void vertical_intersection(double x1,double y1,double x2, double y2, double x,double &x_intersect, double &y_intersect){
    x_intersect = x;
    y_intersect = y1 + ((y2 - y1) * (x - x1) / (x2 - x1));
}
void horizontal_intersection(double x1,double y1,double x2, double y2, double y,double &x_intersect, double &y_intersect){
    y_intersect = y;
    x_intersect = x1 + ((x2 - x1) * (y - y1) / (y2 - y1));
}

// --------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------


void ClipLineRectangle(HDC hdc, int xs, int ys, int xe, int ye,int xleft , int ytop , int xright , int ybottom){


    double x1 = xs, y1 = ys, x2 = xe, y2 = ye;
    OutCode out1 = getOutCode(x1,y1,xleft,xright,ytop,ybottom);
    OutCode out2 = getOutCode(x2,y2,xleft,xright,ytop,ybottom);

    while( (out1.all || out2.all) && ! (out1.all & out2.all)){
        double xi,yi;
        if(out1.all){
            if (out1.left) 
                vertical_intersection(x1,y1,x2,y2,xleft,xi,yi);
            else if(out1.top) 
                horizontal_intersection(x1,y1,x2,y2,ytop,xi,yi);
            else if(out1.right) 
                vertical_intersection(x1,y1,x2,y2,xright,xi,yi);
            else 
                horizontal_intersection(x1,y1,x2,y2,ybottom,xi,yi);
            x1 = xi;
            y1 = yi;
            out1 = getOutCode(x1,y1,xleft,xright,ytop,ybottom);
        }
        else {
            if (out2.left) 
                vertical_intersection(x1,y1,x2,y2,xleft,xi,yi);
            else if(out2.top) 
                horizontal_intersection(x1,y1,x2,y2,ytop,xi,yi);
            else if(out2.right) 
                vertical_intersection(x1,y1,x2,y2,xright,xi,yi);
            else 
                horizontal_intersection(x1,y1,x2,y2,ybottom,xi,yi);
            x2 = xi;
            y2 = yi;
            out2 = getOutCode(x2,y2,xleft,xright,ytop,ybottom);
        }
    }
    if (!out1.all && !out2.all)
        DrawLineDDA(hdc,(int)x1,(int)y1,(int)x2,(int)y2,RGB(255,0,0));
    
}

// -------------------------------------------


// Mohamed Gamal
void ClipLineSquare(HDC hdc , int xs , int ys , int xe , int ye , int xleft , int ytop , int xright , int ybottom){
    ClipLineRectangle(hdc , xs , ys , xe , ye , xleft , ytop , xright , ybottom);
}
