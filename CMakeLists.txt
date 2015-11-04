cmake_minimum_required(VERSION 3.3)
project(mss_tests)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lgmp -lgmpxx")

set(SOURCE_FILES main.cpp)
add_executable(mss_tests ${SOURCE_FILES})
