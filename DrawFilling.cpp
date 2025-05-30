#include <windows.h>
#include <list>
#include <cmath>
using namespace std;

struct Point {
    int x, y;
    Point(int x = 0 , int y = 0) : x(x) , y(y) {}
};

void swap(int& x , int& y) {
    int temp = x;
    x = y;
    y = temp;
}

void swap(Point& p1 , Point& p2) {
    swap(p1.x , p2.x);
    swap(p1.y , p2.y);
}


// -------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------

// Ahmed Mohsen
void FillCircleWithLines(HDC hdc ,int xc, int yc, int r, int quarter){
    
}

// -------------------------------------------

// Osama
void FillCircleWithCircles(HDC hdc ,int xc, int yc, int r, int quarter){

}

// -------------------------------------------

// Anas Adel
void FillSquareWithHermite(HDC hdc){

}

// -------------------------------------------

// Mohamed Gamal
void FillRectangleWithBezier(HDC hdc){

}

// -------------------------------------------
// -------------------------------------------
// -------------------------------------------
// Mohamed Gamal

struct Entry {
    int xmin , xmax ;
};

void InitEntrites(Entry table[]) {
    for (int i = 0 ; i < 800 ; i++) {
        table[i].xmin = INT_MAX ;
        table[i].xmax = INT_MIN ;
    }
}

void ScanEdge(Point p1 , Point p2 , Entry table[]) {
    if (p1.y == p2.y) return ;
    if (p1.y > p2.y) swap(p1,p2);
    double mInverse = (double) (p2.x - p1.x) / (p2.y - p1.y) ; 
    double x = p1.x ;
    int y = p1.y ; 
    while (y < p2.y) {
        if (x < table[y].xmin) table[y].xmin = x ;
        if (x > table[y].xmax) table[y].xmax = x ;
        x += mInverse ;
        y++ ;
    }
}

void DrawScanLine(HDC hdc , Entry table[] , COLORREF color) {
    for (int i = 0 ; i < 800 ; i++){
        if (table[i].xmin <= table[i].xmax) {
            for (int x = table[i].xmin ; x <= table[i].xmax ; x++) {
                SetPixel(hdc , x , i , color);
            }
        }
    }
}

void ConvexFilling(HDC hdc , Point p[] , int n , COLORREF color){
    Entry *table = new Entry[800];
    InitEntrites(table);
    Point p1 = p[n-1];
    for (int i = 0 ; i < n ; i++) {
        Point p2 = p[i];
        ScanEdge(p1 , p2 , table);
        p1 = p2;
    }
    DrawScanLine(hdc , table , color);
    delete table ; 
}

// -------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------

// Osama
struct PolygonPoint{
    int x, y;
    PolygonPoint(int x = 0, int y = 0):x(x),y(y){};
};

void swap(PolygonPoint& p1, PolygonPoint& p2){
    swap(p1.x,p2.x);
    swap(p1.y,p2.y);
}
struct EdgeInfo{
    double x , m_inverse;
    int y_max;
    bool operator<(EdgeInfo& n2){
        return x < n2.x;
    }
    EdgeInfo(double x = 0 ,double m_inverse = 0, int y = 0):x(x),m_inverse(m_inverse),y_max(y){};
};


typedef std::list<EdgeInfo> EdgeList;
typedef EdgeList EdgeTable[800];


void edgeToTable(PolygonPoint& p1,PolygonPoint& p2,EdgeTable tbl){
    if(p1.y == p2.y) return;
    if(p1.y > p2.y) swap(p1,p2);
    double m_inverse = (double) (p2.x - p1.x) / (p2.y - p1.y);
    tbl[p1.y].push_back(EdgeInfo(p1.x,m_inverse,p2.y));
};


void polygonToTable(PolygonPoint points[] ,int n,EdgeTable tbl){
    PolygonPoint p1 = points[n-1];
    for (int i = 0; i < n; ++i) {
        PolygonPoint p2 = points[i];
        edgeToTable(p1,p2,tbl);
        p1 = points[i];
    }
}


void NonConvexFilling(HDC hdc,PolygonPoint points[],int n,COLORREF color){
    // initialize
    EdgeTable table;
    polygonToTable(points,n,table);
    int y = 0;
    while(y < 800 && table[y].empty()) y++;
    if(y==800) return;
    EdgeList active = table[y];
    while (!active.empty()){
        active.sort();
        // draw horizontal line
        for(auto it = active.begin();it != active.end();it++){
            int x1=(int)ceil(it->x);
            it++;
            int x2=(int)floor(it->x);
            for(int x=x1;x<=x2;x++)SetPixel(hdc,x,y,color);
        }
        y++;
        // delete edge when it is done
        EdgeList::iterator it = active.begin();
        while (it != active.end()){
            if(it->y_max == y)it = active.erase(it);
            else it++;
        }
        // update
        for(auto it = active.begin();it != active.end();it++){
            it->x += it->m_inverse;
        }
        // append
        active.insert(active.end(),table[y].begin(),table[y].end());
    }
}