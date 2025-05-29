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

// Mohamed Gamal
void ClipPolygonRectangle(HDC hdc  , Vertex *p , int n , int xleft , int ytop , int xright , int ybottom){
    VertexList plist ;
    for (int i = 0 ; i < n ; i++)
        plist.push_back(Vertex(p[i].x , p[i].y));
    plist = ClipWithEdge(plist , xleft , IsLeft , VIntersection);
    plist = ClipWithEdge(plist , ytop , IsTop , HIntersection);
    plist = ClipWithEdge(plist , xright , IsRight , VIntersection);
    plist = ClipWithEdge(plist , ybottom , IsBottom , HIntersection);
    
    Vertex v1 = plist[plist.size() - 1];
    for (int i = 0 ; i < plist.size() ; i++){
        Vertex v2 = plist[i];
        MoveToEx(hdc , v1.x , v1.y , NULL);
        LineTo(hdc , v2.x , v2.y);
        v1 = v2 ; 
    }
}

