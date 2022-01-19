cmake_minimum_required(VERSION 3.10)

project("TriangleTest")


file(GLOB_RECURSE RES_FILES  ${CMAKE_CURRENT_LIST_DIR}/*.hlsl*)


SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_LIST_DIR})


SET(MEWA_SRC_DIR ${CMAKE_CURRENT_LIST_DIR}/../../../..)
include(${MEWA_SRC_DIR}/mewa.cmake)


add_executable(TriangleTest 
    triangleapp.cpp
    main.cpp 
    ${SOURCE}
)

target_link_libraries( TriangleTest
    PUBLIC d3d12.lib
    PUBLIC dxgi.lib
)

