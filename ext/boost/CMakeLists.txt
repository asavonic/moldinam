cmake_minimum_required(VERSION 2.8.7)
set(PROJECT_NAME_STR boost_parts)
project(${PROJECT_NAME_STR} C CXX)

if(CMAKE_COMPILER_IS_GNUCXX)
	add_definitions(-Wall -ansi -Wno-deprecated -pthread -std=c++11 -O0 )
endif()

if(MSVC)
#vc 2012 fix for vararg templates
    set(MSVC_COMPILER_DEFS "-D_VARIADIC_MAX=10")
endif() 


include_directories( program_options/include )
file(GLOB SRC_FILES program_options/src/*.cpp)

add_library(boost_program_options ${SRC_FILES})
