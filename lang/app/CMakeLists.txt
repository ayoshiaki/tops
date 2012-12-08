cmake_minimum_required(VERSION 2.6)

include_directories(../src ../../src)

add_executable(parser parser.cpp)
target_link_libraries(parser tops-lang)



