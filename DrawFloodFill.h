#ifndef DRAW_FLOOD_FILL_H
#define DRAW_FLOOD_FILL_H

#include <windows.h>

void RecursiveFloodFill(HDC hdc, int x, int y, COLORREF bc, COLORREF fc);
void NonRecursiveFloodFill(HDC hdc, int x, int y, COLORREF bc, COLORREF fc);

#endif // DRAW_FLOOD_FILL_H 