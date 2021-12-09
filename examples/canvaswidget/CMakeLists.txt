cmake_minimum_required(VERSION 3.10)

project(canvaswidget)
    
include(${CMAKE_CURRENT_SOURCE_DIR}/../../src/mewa.cmake)

add_executable( canvaswidget 
    canvaswidget.cpp 
    main_win.cpp
    ${SOURCE} 
)


