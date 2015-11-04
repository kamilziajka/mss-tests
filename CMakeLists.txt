cmake_minimum_required(VERSION 3.3)
project(mss_tests)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp)
add_executable(mss_tests ${SOURCE_FILES})
target_link_libraries(mss_tests gmpxx gmp)
