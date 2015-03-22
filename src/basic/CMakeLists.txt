cmake_minimum_required(VERSION 2.8.7)
set(PROJECT_NAME_STR exec)
project(${PROJECT_NAME_STR} C CXX)


include_directories(${Boost_INCLUDE_DIRS})

add_executable(moldynam_basic moldynam_basic.cpp)

get_property(LIB_NAME GLOBAL PROPERTY PROJECT_LIB_NAME)
target_link_libraries(moldynam_basic ${LIB_NAME} )
target_link_libraries(moldynam_basic ${Boost_LIBRARIES})

#installation of executable files
INSTALL(TARGETS moldynam_basic
	RUNTIME DESTINATION "bin"
)
