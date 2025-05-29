#include <windows.h>
#include "DrawLine.cpp"

// Utalitiy Functions
union OutCode{
    unsigned all : 4;
    struct{
        unsigned left : 1, right : 1,top : 1, bottom : 1;
    };
};
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

// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------


// Anas Adel
void ClipPointRectangle(HDC hdc){

}

// -------------------------------------------


void ClipLineRectangle(HDC hdc, int xs, int ys, int xe, int ye,int x_left,int x_right,int y_top,int y_bottom){
    DrawLineDDA(hdc,x_left,y_top,x_right,y_top,RGB(0,0,0));
    DrawLineDDA(hdc,x_left,y_top,x_left,y_bottom,RGB(0,0,0));
    DrawLineDDA(hdc,x_left,y_bottom,x_right,y_bottom,RGB(0,0,0));
    DrawLineDDA(hdc,x_right,y_top,x_right,y_bottom,RGB(0,0,0));
    double x1 = xs, y1 = ys, x2 = xe, y2 = ye;
    OutCode out1 = getOutCode(x1,y1,x_left,x_right,y_top,y_bottom);
    OutCode out2 = getOutCode(x2,y2,x_left,x_right,y_top,y_bottom);
    while( (out1.all || out2.all) && ! (out1.all & out2.all)){
        double xi,yi;
        if(out1.all){
            if (out1.left) vertical_intersection(x1,y1,x2,y2,x_left,xi,yi);
            else if(out1.top) horizontal_intersection(x1,y1,x2,y2,y_top,xi,yi);
            else if(out1.right) vertical_intersection(x1,y1,x2,y2,x_right,xi,yi);
            else horizontal_intersection(x1,y1,x2,y2,y_bottom,xi,yi);
            x1 = xi;
            y1 = yi;
            out1 = getOutCode(x1,y1,x_left,x_right,y_top,y_bottom);
        }
        else {
            if (out2.left) vertical_intersection(x1,y1,x2,y2,x_left,xi,yi);
            else if(out2.top) horizontal_intersection(x1,y1,x2,y2,y_top,xi,yi);
            else if(out2.right) vertical_intersection(x1,y1,x2,y2,x_right,xi,yi);
            else horizontal_intersection(x1,y1,x2,y2,y_bottom,xi,yi);
            x2 = xi;
            y2 = yi;
            out2 = getOutCode(x2,y2,x_left,x_right,y_top,y_bottom);
        }
    }
    if (!out1.all && !out2.all){
        DrawLineDDA(hdc,(int)x1,(int)y1,(int)x2,(int)y2,RGB(255,0,0));
    }
}

// -------------------------------------------

// Mohamed Gamal
void ClipPolygonRectangle(HDC hdc){


}

// -------------------------------------------

// Ahmed Mohsen
void ClipPointSquare(HDC hdc){


}

// -------------------------------------------

// Mohamed Gamal
void ClipLineSquare(HDC hdc){


}
