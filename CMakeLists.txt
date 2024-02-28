cmake_minimum_required(VERSION 3.0)
project(libkore)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(Source/ThirdParty)

add_library(kore STATIC
    Source/Public/Assertion.hpp
    Source/Public/Exceptions/RuntimeException.hpp
    Source/Private/Exceptions/RuntimeException.cpp
    Source/Public/Utilities/Strings.hpp
    Source/Private/Utilities/Strings.cpp
)

target_include_directories(kore PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/Public
)

target_link_libraries(kore PUBLIC 
    backward
    fmt
)
