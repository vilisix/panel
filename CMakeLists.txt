cmake_minimum_required(VERSION 3.20)
project(panel)

project(panel LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build)

add_subdirectory(hotline)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/hotline/src)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/hotline/libs/imgui)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/hotline/libs/imgui/backends)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/hotline/libs/glfw/include)

## Create panel executable
add_executable(panel src/main.cpp)
target_link_directories(panel PUBLIC main)
target_link_libraries(
  panel
  PUBLIC
	hotline
    glfw
    IMGUI
)