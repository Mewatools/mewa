

if (UNIX)
    include_directories(
        ${CMAKE_CURRENT_LIST_DIR}/platformabstraction/linux
    )
endif (UNIX)

if (MSVC) # or should it be WIN32 ??
    include_directories(
        ${CMAKE_CURRENT_LIST_DIR}/platformabstraction/windows
    )
endif (MSVC)



include_directories(
    ${CMAKE_CURRENT_LIST_DIR}/containers
    ${CMAKE_CURRENT_LIST_DIR}/events
    ${CMAKE_CURRENT_LIST_DIR}/gui
    ${CMAKE_CURRENT_LIST_DIR}/gui/icondraw
    ${CMAKE_CURRENT_LIST_DIR}/gui/renderer
    ${CMAKE_CURRENT_LIST_DIR}/gui/textdraw
    ${CMAKE_CURRENT_LIST_DIR}/gui/vectordraw
    ${CMAKE_CURRENT_LIST_DIR}/iconatlas
    ${CMAKE_CURRENT_LIST_DIR}/geometry
)

list(APPEND SOURCE
    ${CMAKE_CURRENT_LIST_DIR}/events/mxapplication.cpp
    ${CMAKE_CURRENT_LIST_DIR}/events/mxmouseevent.cpp
    ${CMAKE_CURRENT_LIST_DIR}/gui/mxwidget.cpp
)

