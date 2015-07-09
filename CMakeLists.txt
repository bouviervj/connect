CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

PROJECT(ClientConnect C CXX)

ADD_DEFINITIONS(-g -O0)

include_directories ($(ClientConnect_SOURCE_DIR)) 

ADD_SUBDIRECTORY(lib)

file(GLOB main_sources ./main.c)

SET(GCC_COVERAGE_COMPILE_FLAGS "-fstack-protector-all -fPIC -finstrument-functions")
SET(GCC_COVERAGE_LINK_FLAGS    "")      

SET( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}" )
SET( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}" )
SET( CMAKE_INCLUDE_PATH "${CMAKE_INCLUDE_PATH} ${GCC_COVERAGE_INCLUDE}" ) 

find_package(Threads REQUIRED)

SET( LIBS ${CMAKE_THREAD_LIBS_INIT} )

add_executable(main ${main_sources} )
target_link_libraries(main ${LIBS} )
