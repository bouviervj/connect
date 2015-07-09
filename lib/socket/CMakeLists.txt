CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

file(GLOB common_sources ./*.cpp)

add_library (socket ${common_sources})
