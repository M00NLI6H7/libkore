cmake_minimum_required(VERSION 3.0)
project(libkore)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(ThirdParty)

add_library(kore STATIC
    Include/Exceptions/RuntimeException.hpp
    Source/Exceptions/RuntimeException.cpp
    Include/Utilities/Strings.hpp
    Source/Utilities/Strings.cpp
)

target_include_directories(kore PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/Include
)

target_link_libraries(kore PUBLIC 
    backward
    fmt
)
