cmake_minimum_required(VERSION 3.10)

project (canvaswidget)
 
include_directories(
../../src/containers
../../src/events
../../src/geometry
../../src/gui
)

add_executable(canvaswidget canvaswidget.cpp main.cpp)




