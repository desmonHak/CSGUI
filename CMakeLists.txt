cmake_minimum_required(VERSION 3.28)
project(CSGUI)

set(CMAKE_CXX_STANDARD 14)

include_directories(include src)

add_executable(code_c
        include/csgui.h
        include/csgui.hpp
        src/csgui.c
        src/csgui.cpp
        code.c)

add_executable(code_cpp
        include/csgui.h
        include/csgui.hpp
        src/csgui.c
        src/csgui.cpp
        code.cpp)
