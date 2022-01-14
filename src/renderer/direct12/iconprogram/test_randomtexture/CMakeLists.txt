cmake_minimum_required(VERSION 3.10)

project("RandomTexture")


file(GLOB_RECURSE RES_FILES  ${CMAKE_CURRENT_LIST_DIR}/*.hlsl*)
#include(${CMAKE_CURRENT_LIST_DIR}/../iconprogram.cmake)



SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_LIST_DIR})


SET(MEWA_SRC_DIR ${CMAKE_CURRENT_LIST_DIR}/../../../..)
include(${MEWA_SRC_DIR}/mewa.cmake)


add_executable(RandomTexture 
    randomtextureapp.cpp
    main.cpp 
    ${SOURCE}
)

target_link_libraries( RandomTexture
    PUBLIC d3d12.lib
    PUBLIC dxgi.lib
)

