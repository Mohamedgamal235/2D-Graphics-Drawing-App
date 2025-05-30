#include <windows.h>
#include <vector>
using namespace std;

struct Vertex{
    double x , y ; 
    Vertex(double x , double y){
        this->x = x ; 
        this->y = y ; 
    }
};

typedef vector<Vertex> VertexList;
typedef bool (*IsIn)(Vertex & v , int edge) ;    
typedef Vertex (*GetIntersectionFunc)(Vertex & v1 , Vertex & v2 , int edge) ;


bool IsLeft(Vertex & v , int edge ){
    return v.x >= edge ; 
}

bool IsRight(Vertex & v , int edge ){
    return v.x <= edge ; 
}

bool IsTop(Vertex & v , int edge ){
    return v.y >= edge ; 
}

bool IsBottom(Vertex & v , int edge ){
    return v.y <= edge ; 
}

Vertex VIntersection(Vertex & v1 , Vertex & v2 , int xedge ){
    double y = v1.y + (xedge - v1.x) * (v2.y - v1.y) / (v2.x - v1.x);
    return Vertex(xedge, y);    
}

Vertex HIntersection(Vertex & v1 , Vertex & v2 , int yedge ){
    double x = v1.x + (yedge - v1.y) * (v2.x - v1.x) / (v2.y - v1.y);
    return Vertex(x, yedge);
}


VertexList ClipWithEdge(VertexList p , int edge , IsIn In , GetIntersectionFunc Intersect){
    VertexList outList ; 
    Vertex v1 = p[p.size() - 1];

    bool v1_in = In(v1 , edge);
    for (int i = 0 ; i < p.size() ; i++){
        Vertex v2 = p[i];
        bool v2_in = In(v2 , edge);
        if (!v1_in && v2_in){
            outList.push_back(Intersect(v1 , v2 , edge));
            outList.push_back(v2);
        }
        else if (v1_in && v2_in)
            outList.push_back(v2);
        else if (v1_in && !v2_in)
            outList.push_back(Intersect(v1 , v2 , edge));
        v1 = v2 ; 
        v1_in = v2_in ; 
    }
    return outList ; 
}

void DrawPolygon(HDC hdc , VertexList p , COLORREF color){
    if (p.size() < 2) return ; 

    HPEN pen = CreatePen(PS_SOLID , 2 , color);
    HPEN oldPen = (HPEN)SelectObject(hdc , pen);

    MoveToEx(hdc , (int)p[0].x , (int)p[0].y , NULL);
    for (int i = 1 ; i < p.size() ; i++)
        LineTo(hdc , (int)p[i].x , (int)p[i].y);

    LineTo(hdc , (int)p[0].x , (int)p[0].y);

    SelectObject(hdc, oldPen);
    DeleteObject(pen);
}

void ClipPolygonRectangle(HDC hdc  , Vertex p[] , int n , int xleft , int ytop , int xright , int ybottom){
    VertexList plist ;
    for (int i = 0 ; i < n ; i++)
        plist.push_back(Vertex(p[i].x , p[i].y));
    
    plist = ClipWithEdge(plist , xleft , IsLeft , VIntersection);
    plist = ClipWithEdge(plist , ytop , IsTop , HIntersection);
    plist = ClipWithEdge(plist , xright , IsRight , VIntersection);
    plist = ClipWithEdge(plist , ybottom , IsBottom , HIntersection);
    
    DrawPolygon(hdc , plist , RGB(255 , 0 , 0));
}

