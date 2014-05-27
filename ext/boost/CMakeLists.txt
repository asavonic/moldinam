cmake_minimum_required(VERSION 2.8.7)
set(PROJECT_NAME_STR boost_parts)
project(${PROJECT_NAME_STR} C CXX)

if ( WIN32 ) 
    set ( BOOST_BOOTSTRAP ${CMAKE_CURRENT_LIST_DIR}/bootstrap.bat )
else()
    set ( BOOST_BOOTSTRAP ${CMAKE_CURRENT_LIST_DIR}/bootstrap.sh )
endif()

if ( NOT EXISTS  ${CMAKE_CURRENT_LIST_DIR}/b2 )
    execute_process( COMMAND  ${BOOST_BOOTSTRAP}
                     WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} )
endif()

execute_process( COMMAND  ${CMAKE_CURRENT_LIST_DIR}/b2 --link --layout=system --variant=release --runtime-link=shared --libdir=${PROJECT_BINARY_DIR} install 
                 WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} )

set( BOOST_PROGRAM_OPTIONS ${CMAKE_CURRENT_BINARY_DIR}/libboost_program_options.a PARENT_SCOPE )
