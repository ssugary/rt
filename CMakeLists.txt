# To make the LSP work, also needed to:
# rm -rf build
# cmake -B build
# ln -s build/compile_commands.json .

cmake_minimum_required(VERSION 3.16)
project(rt)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Debug) # just for gdb

set(CMAKE_BUILD_TYPE Release) # for fastier math calcs
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -march=native -flto -ffast-math")

find_package(OpenMP REQUIRED) # lib to divide the render's loop into threads

file(GLOB_RECURSE SRC_FILES
    ${CMAKE_SOURCE_DIR}/src/core/*.cpp
    ${CMAKE_SOURCE_DIR}/src/core/integrators/*.cpp
    ${CMAKE_SOURCE_DIR}/src/core/materials/*.cpp
    ${CMAKE_SOURCE_DIR}/src/core/primitives/*.cpp
    ${CMAKE_SOURCE_DIR}/src/core/light/*.cpp
    ${CMAKE_SOURCE_DIR}/src/msg_system/*.cpp
)

# Libs that need to compile
set(LIB_SOURCES
    ${CMAKE_SOURCE_DIR}/lib/lodepng/lodepng.cpp
    ${CMAKE_SOURCE_DIR}/lib/lodepng/lodepng_util.cpp
    ${CMAKE_SOURCE_DIR}/lib/tinyxml2/tinyxml2.cpp
)

add_executable(${PROJECT_NAME}
    ${SRC_FILES}
    ${LIB_SOURCES}
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/core
    ${CMAKE_SOURCE_DIR}/src/core/integrators
    ${CMAKE_SOURCE_DIR}/src/core/materials
    ${CMAKE_SOURCE_DIR}/src/core/light
    ${CMAKE_SOURCE_DIR}/src/core/primitives
    ${CMAKE_SOURCE_DIR}/src/msg_system
    ${CMAKE_SOURCE_DIR}/lib
)

target_link_libraries(${PROJECT_NAME} PRIVATE OpenMP::OpenMP_CXX) # lib to divide the render's loop into threads

target_compile_options(${PROJECT_NAME} PRIVATE
    -Wall -Wextra -Wpedantic -fopenmp
)                           #-fopenmp : flag to use OpenMP lib
