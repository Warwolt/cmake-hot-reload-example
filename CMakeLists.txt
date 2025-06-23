cmake_minimum_required(VERSION 4.0.2)
project(Example C CXX)

set(CMAKE_CXX_STANDARD 23)

# Source
add_library(MyLib OBJECT src/mylib.cpp)
target_include_directories(MyLib PUBLIC include)

# Debug DLL
add_library(MyLibShared SHARED)
target_link_libraries(MyLibShared PUBLIC debug MyLib)

# Release lib
add_library(MyLibStatic STATIC)
target_link_libraries(MyLibStatic PUBLIC optimized MyLib)
set_property(TARGET MyLibStatic PROPERTY LINKER_LANGUAGE CXX)

# Executable
add_executable(MyApp src/main.cpp)
target_link_libraries(MyApp
    debug MyLibShared
    optimized MyLibStatic)
