cmake_minimum_required(VERSION 3.10)

project("RandomTexture")



SET(MEWA_SRC_DIR ${CMAKE_CURRENT_LIST_DIR}/../../../..)
include(${MEWA_SRC_DIR}/mewa.cmake)

SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_LIST_DIR})


add_executable(RandomTexture  WIN32
    randomtextureapp.cpp
    main.cpp 
    ${SOURCE}
)


ADD_CUSTOM_TARGET(link_target ALL
  COMMAND ${CMAKE_COMMAND} -E create_symlink 
  ${MEWA_SRC_DIR}/renderer/direct12/hlsl
  ${EXECUTABLE_OUTPUT_PATH}/hlsl )


file(GLOB_RECURSE RES_FILES  ${CMAKE_CURRENT_LIST_DIR}/hlsl/*.hlsl*)

target_link_libraries( RandomTexture
    PUBLIC d3d12.lib
    PUBLIC dxgi.lib
)

