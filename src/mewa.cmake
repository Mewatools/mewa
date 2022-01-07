

if (UNIX)
    message(STATUS "OpenGL renderer")
    add_compile_definitions(MX_OPENGL_RENDERER)
    include_directories(
        ${CMAKE_CURRENT_LIST_DIR}/platformabstraction/linux
    )
endif (UNIX)

if (MSVC) # or should it be WIN32 ??
    message(STATUS "DirectX 12 renderer")
    add_compile_definitions(MX_DIRECTX12_RENDERER)
    include_directories(
        ${CMAKE_CURRENT_LIST_DIR}/platformabstraction/windows
        ${CMAKE_CURRENT_LIST_DIR}/renderer/direct12
        ${CMAKE_CURRENT_LIST_DIR}/renderer/direct12/colorwheelprogram
        ${CMAKE_CURRENT_LIST_DIR}/renderer/direct12/iconprogram
        ${CMAKE_CURRENT_LIST_DIR}/renderer/direct12/vectorprogram
    )
    
    list(APPEND SOURCE
        ${CMAKE_CURRENT_LIST_DIR}/renderer/direct12/mxrenderer.cpp
        ${CMAKE_CURRENT_LIST_DIR}/renderer/direct12/mxgpuarray.cpp
        ${CMAKE_CURRENT_LIST_DIR}/renderer/direct12/mxgpuprogram.cpp
        ${CMAKE_CURRENT_LIST_DIR}/renderer/direct12/mxtexture.cpp
        ${CMAKE_CURRENT_LIST_DIR}/renderer/direct12/vectorprogram/mxvectorprogram.cpp
        ${CMAKE_CURRENT_LIST_DIR}/renderer/direct12/iconprogram/mxiconprogram.cpp
    )
endif (MSVC)



include_directories(
    ${CMAKE_CURRENT_LIST_DIR}/containers
    ${CMAKE_CURRENT_LIST_DIR}/events
    ${CMAKE_CURRENT_LIST_DIR}/gui
    ${CMAKE_CURRENT_LIST_DIR}/iconatlas
    ${CMAKE_CURRENT_LIST_DIR}/geometry
)

list(APPEND SOURCE
    ${CMAKE_CURRENT_LIST_DIR}/events/mxapplication.cpp
    ${CMAKE_CURRENT_LIST_DIR}/events/mxmouseevent.cpp
    ${CMAKE_CURRENT_LIST_DIR}/geometry/mxmatrix.cpp
     ${CMAKE_CURRENT_LIST_DIR}/iconatlas/mxabstractatlas.cpp
###### GUI #######
    ${CMAKE_CURRENT_LIST_DIR}/gui/mxbuffer.cpp
    ${CMAKE_CURRENT_LIST_DIR}/gui/mxguiaggregation.cpp
    ${CMAKE_CURRENT_LIST_DIR}/gui/mxguirenderer.cpp
    ${CMAKE_CURRENT_LIST_DIR}/gui/mxpainter.cpp
    ${CMAKE_CURRENT_LIST_DIR}/gui/mxquaddraw.cpp
    ${CMAKE_CURRENT_LIST_DIR}/gui/mxsvgpainter.cpp
    ${CMAKE_CURRENT_LIST_DIR}/gui/mxtextdraw.cpp
    ${CMAKE_CURRENT_LIST_DIR}/gui/mxvectordraw.cpp
    ${CMAKE_CURRENT_LIST_DIR}/gui/mxicondraw.cpp
    ${CMAKE_CURRENT_LIST_DIR}/gui/mxwidget.cpp
)

