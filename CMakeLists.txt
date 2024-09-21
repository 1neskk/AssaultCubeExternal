cmake_minimum_required(VERSION 3.17)
project(ACExternal CXX)

set(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp")
add_executable(${PROJECT_NAME} ${SOURCES})

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/imgui)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/glm)

set(IMGUI_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/imgui/imgui.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/imgui/imgui_demo.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/imgui/imgui_draw.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/imgui/imgui_tables.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/imgui/imgui_widgets.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/imgui/backends/imgui_impl_win32.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/imgui/backends/imgui_impl_dx11.cpp
)

add_library(imgui STATIC ${IMGUI_SRC})
target_link_libraries(${PROJECT_NAME} imgui)
target_link_libraries(${PROJECT_NAME} d3d11)

set_property(TARGET ${PROJECT_NAME} PROPERTY WIN32_EXECUTABLE TRUE)