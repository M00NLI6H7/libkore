cmake_minimum_required(VERSION 3.0)
project(libkore)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(ThirdParty)

add_library(libkore STATIC
    include/Utilities/Strings.hpp
    source/Utilities/Strings.cpp
)

target_include_directories(libkore PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
