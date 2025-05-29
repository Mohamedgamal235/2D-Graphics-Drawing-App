#include <windows.h>


void DrawRectungle(HDC hdc , int left , int top , int right , int bottom){
    HPEN pen = CreatePen(PS_SOLID , 3 , RGB(0 , 0 , 0));
    HPEN oldPen = (HPEN)SelectObject(hdc , pen);

    Rectangle(hdc , left , top , right , bottom);

    SelectObject(hdc , oldPen);
    DeleteObject(pen);
}
