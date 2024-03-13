# Copyright (c) 2024 M00NLI6H7 (M00NLI6H7@protonmail.com)
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:

# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

cmake_minimum_required(VERSION 3.0)
project(libkore)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(Source/ThirdParty)

option(LIBKORE_SHARED "Build dynamic libkore as shared object" OFF)

if(LIBKORE_SHARED)
    set(LIBKORE_TYPE SHARED)
endif()

add_library(kore ${LIBKORE_TYPE}
    Source/Public/Assertion.hpp
    Source/Public/Exceptions/RuntimeException.hpp
    Source/Private/Exceptions/RuntimeException.cpp
    Source/Public/Stream/Stream.hpp
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
