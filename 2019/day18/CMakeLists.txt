cmake_minimum_required(VERSION 3.15)

project(Day18)

enable_testing()

set(CMAKE_CXX_FLAGS "-std=c++17 -g")
add_executable(day18 main.cpp)
target_link_libraries(day18 -lpthread)

add_executable(test_intcode IntcodeTests.cpp)
target_link_libraries(test_intcode -lpthread)

add_test(intcodetest test_intcode)
