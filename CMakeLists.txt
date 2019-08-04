cmake_minimum_required(VERSION 3.8)
project(bier)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wall -Wno-sign-compare")

find_package(LLVM)

set(BIER_INC ${CMAKE_CURRENT_SOURCE_DIR} CACHE STRING "bier inc dir")

add_subdirectory(bier)

