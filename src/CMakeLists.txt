cmake_minimum_required(VERSION 2.8.7)
set(PROJECT_NAME_STR exec)
project(${PROJECT_NAME_STR} C CXX)


add_executable(moldinam_basic moldinam_basic.cpp)

target_link_libraries( moldinam_basic ${PROJECT_LIB_NAME} ${BOOST_PROGRAM_OPTIONS} )

#installation of executable files
INSTALL ( FILES
          ${CMAKE_CURRENT_BINARY_DIR}/moldinam_basic 

          PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_READ GROUP_EXECUTE
          DESTINATION ${CMAKE_INSTALL_PREFIX}
        )
