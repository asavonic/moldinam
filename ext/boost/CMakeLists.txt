cmake_minimum_required(VERSION 2.8.7)
set(PROJECT_NAME_STR boost_parts)
project(${PROJECT_NAME_STR} C CXX)

execute_process( COMMAND  ./bootstrap.sh 
                 WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} )

execute_process( COMMAND  ./b2 --link --threading=single --variant=release --runtime-link=shared --libdir=${PROJECT_BINARY_DIR} install 
                 WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} )

set( BOOST_PROGRAM_OPTIONS ${CMAKE_CURRENT_BINARY_DIR}/libboost_program_options.a PARENT_SCOPE )
