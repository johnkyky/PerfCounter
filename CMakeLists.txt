cmake_minimum_required(VERSION 3.27)
project(perfTest)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(src)

add_executable(main main.cpp)
target_link_libraries(main perf)
