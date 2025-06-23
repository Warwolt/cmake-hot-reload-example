cmake_minimum_required(VERSION 4.0.2)
project(Example)

set(CMAKE_CXX_STANDARD 23)

add_library(MyLib OBJECT EXCLUDE_FROM_ALL src/mylib.cpp)
target_include_directories(MyLib PUBLIC include)

add_library(MyLibShared SHARED EXCLUDE_FROM_ALL)
target_link_libraries(MyLibShared PUBLIC MyLib)

add_library(MyLibStatic STATIC EXCLUDE_FROM_ALL)
target_link_libraries(MyLibStatic PUBLIC MyLib)

add_executable(MyApp src/main.cpp)
target_link_libraries(MyApp
    debug MyLibShared
    optimized MyLibStatic)
