cmake_minimum_required(VERSION 3.15)

project(Day9)

enable_testing()

set(CMAKE_CXX_FLAGS "-std=c++17")
add_executable(day9 main.cpp)

add_executable(test_intcode IntcodeTests.cpp)
target_link_libraries(test_intcode -lpthread)

add_test(intcodetest test_intcode)
