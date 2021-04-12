cmake_minimum_required(VERSION 3.15)

project(Day15)

enable_testing()

set(CMAKE_CXX_FLAGS "-std=c++17 -g")
add_executable(day15 main.cpp)
target_link_libraries(day15 -lpthread)

add_executable(test_intcode IntcodeTests.cpp)
target_link_libraries(test_intcode -lpthread)

add_test(intcodetest test_intcode)
