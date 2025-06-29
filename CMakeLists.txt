cmake_minimum_required(VERSION 4.0.2)
project(MyApp C CXX)

set(CMAKE_CXX_STANDARD 23)

# Debug DLL
add_library(MyLibShared SHARED src/mylib.cpp)
set_target_properties(MyLibShared PROPERTIES OUTPUT_NAME MyLib)
target_include_directories(MyLibShared PUBLIC src)

# # Static library
add_library(MyLibStatic STATIC)
target_sources(MyLibStatic PRIVATE
    $<$<CONFIG:Debug>:src/mylib_load.cpp>
    $<$<CONFIG:Release>:src/mylib.cpp>
)
set_target_properties(MyLibStatic PROPERTIES OUTPUT_NAME MyLib)
target_include_directories(MyLibStatic PUBLIC src)

# # Executable
add_executable(MyApp src/main.cpp src/run_command.cpp)
target_link_libraries(MyApp MyLibStatic)
target_include_directories(MyApp PUBLIC src)
set_property(TARGET MyApp PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT MyApp)
