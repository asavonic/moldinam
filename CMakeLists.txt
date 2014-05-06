cmake_minimum_required(VERSION 2.8.7)
set(PROJECT_NAME_STR moldinam)
project(${PROJECT_NAME_STR} C CXX)

if(CMAKE_COMPILER_IS_GNUCXX)
	add_definitions(-Wall -ansi -Wno-deprecated -pthread -std=c++11)
endif()
if( ${CMAKE_C_COMPILER} MATCHES "icc" )
	add_definitions(-Wall -ansi -Wno-deprecated -pthread -std=c++11 )
endif()

if (CMAKE_BUILD_TYPE EQUAL Debug)
    add_definitions(-O0)
else()
    add_definitions(-O2)
endif()

if(MSVC)
#vc 2012 fix for vararg templates
    set(MSVC_COMPILER_DEFS "-D_VARIADIC_MAX=10")
endif() 
#-------------------
# set common include folder for module
#-------------------
set(COMMON_INCLUDES ${PROJECT_SOURCE_DIR}/include)
set(EXT_PROJECTS_DIR ${PROJECT_SOURCE_DIR}/ext)

#-------------------
# Module source
#-------------------
set(PROJECT_LIB_NAME ${PROJECT_NAME_STR})

include_directories(${COMMON_INCLUDES})
file(GLOB SRC_FILES ${PROJECT_SOURCE_DIR}/lib/*.cpp)
add_library(${PROJECT_LIB_NAME} ${SRC_FILES})

add_subdirectory(ext/gtest-1.7.0)
include_directories(ext/gtest-1.7.0/include)

add_subdirectory(ext/boost)
include_directories(ext/boost)

#-------------------
# Test
#-------------------
add_definitions(${MSVC_COMPILER_DEFS})
enable_testing()
set(PROJECT_TEST_NAME test_${PROJECT_NAME_STR})
include_directories(${COMMON_INCLUDES})
add_subdirectory(test)

#-------------------
# Executables
#-------------------

add_subdirectory(src)
