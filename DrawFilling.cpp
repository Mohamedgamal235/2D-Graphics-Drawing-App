#include "DrawFilling.h"
#include "DrawLine.h"
#include <cmath>
#include <climits>

// Rest of the implementation remains the same...
struct Entry {
    int xmin, xmax;
};

void InitEntries(Entry table[]) {
    for (int i = 0; i < 800; i++) {
        table[i].xmin = INT_MAX;
        table[i].xmax = INT_MIN;
    }
}

void ScanEdge(Point p1, Point p2, Entry table[]) {
    if (p1.y == p2.y) return;
    if (p1.y > p2.y) std::swap(p1, p2);
    double mInverse = (double)(p2.x - p1.x) / (p2.y - p1.y);
    double x = p1.x;
    int y = p1.y;
    while (y < p2.y) {
        if (x < table[y].xmin) table[y].xmin = (int)x;
        if (x > table[y].xmax) table[y].xmax = (int)x;
        x += mInverse;
        y++;
    }
}

void DrawScanLines(HDC hdc, Entry table[], COLORREF color) {
    for (int i = 0; i < 800; i++) {
        if (table[i].xmin <= table[i].xmax) {
            for (int x = table[i].xmin; x <= table[i].xmax; x++) {
                SetPixel(hdc, x, i, color);
            }
        }
    }
}

void ConvexFilling(HDC hdc, Point p[], int n, COLORREF color) {
    Entry* table = new Entry[800];
    InitEntries(table);
    Point p1 = p[n-1];
    for (int i = 0; i < n; i++) {
        Point p2 = p[i];
        ScanEdge(p1, p2, table);
        p1 = p2;
    }
    DrawScanLines(hdc, table, color);
    delete[] table;
}


// ----------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------

struct EdgeInfo {
    double x, m_inverse;
    int y_max;
    bool operator<(EdgeInfo& e2) {
        return x < e2.x;
    }
    EdgeInfo(double x = 0, double m_inverse = 0, int y = 0) : x(x), m_inverse(m_inverse), y_max(y) {}
};

typedef std::list<EdgeInfo> EdgeList;
typedef EdgeList EdgeTable[800];

void EdgeToTable(PolygonPoint& p1, PolygonPoint& p2, EdgeTable tbl) {
    if (p1.y == p2.y) return;
    if (p1.y > p2.y) std::swap(p1, p2);
    double m_inverse = (double)(p2.x - p1.x) / (p2.y - p1.y);
    tbl[p1.y].push_back(EdgeInfo(p1.x, m_inverse, p2.y));
}

void PolygonToTable(PolygonPoint points[], int n, EdgeTable tbl) {
    PolygonPoint p1 = points[n-1];
    for (int i = 0; i < n; i++) {
        PolygonPoint p2 = points[i];
        EdgeToTable(p1, p2, tbl);
        p1 = points[i];
    }
}

void NonConvexFilling(HDC hdc, PolygonPoint points[], int n, COLORREF color) {
    EdgeTable table{};
    PolygonToTable(points, n, table);
    int y = 0;
    while (y < 800 && table[y].empty()) y++;
    if (y == 800) return;
    EdgeList active = table[y];
    while (!active.empty()) {
        active.sort();
        for (auto it = active.begin(); it != active.end(); it++) {
            int x1 = (int)ceil(it->x);
            it++;
            int x2 = (int)floor(it->x);
            for (int x = x1; x <= x2; x++) SetPixel(hdc, x, y, color);
        }
        y++;
        auto it = active.begin();
        while (it != active.end()) {
            if (it->y_max == y) 
                it = active.erase(it);
            else 
                it++;
        }
        for (auto& e : active) {
            e.x += e.m_inverse;
        }
        active.insert(active.end(), table[y].begin(), table[y].end());
    }
} 