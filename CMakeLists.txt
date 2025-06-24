cmake_minimum_required(VERSION 4.0.2)
project(Example C CXX)

set(CMAKE_CXX_STANDARD 23)

# Debug DLL
add_library(MyLibShared SHARED src/mylib.cpp)
set_target_properties(MyLibShared PROPERTIES OUTPUT_NAME MyLib)
target_include_directories(MyLibShared PUBLIC include)
target_compile_definitions(MyLibShared PRIVATE MYLIB_SHARED)

# # Static library
add_library(MyLibStatic STATIC src/mylib.cpp)
set_target_properties(MyLibStatic PROPERTIES OUTPUT_NAME MyLib)
target_compile_definitions(MyLibStatic PRIVATE "$<$<CONFIG:Debug>:IMPORT_LIBRARY>")
target_include_directories(MyLibStatic PUBLIC include)

# # Executable
add_executable(MyApp src/main.cpp)
target_link_libraries(MyApp MyLibStatic)
