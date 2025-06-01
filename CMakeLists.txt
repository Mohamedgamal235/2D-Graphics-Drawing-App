cmake_minimum_required(VERSION 3.28)
project(2D_Drawing_App)

set(CMAKE_CXX_STANDARD 17)

add_executable(2D_Drawing_App 
    RunApp.cpp
    DrawLine.cpp
    DrawCircle.cpp
    DrawEllips.cpp
    DrawFilling.cpp
    DrawFloodFill.cpp
    DrawWindow.cpp
    DrawPolygonClipping.cpp
    DrawPointClipping.cpp
    DrawLineClipping.cpp
    DrawSpline.cpp
)
