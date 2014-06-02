cmake_minimum_required(VERSION 2.8.7)
set(PROJECT_NAME_STR visualizer)
project(${PROJECT_NAME_STR} C CXX)


add_executable(visualizer visualizer.cpp)

include_directories( glm )
include_directories( ${GLUT_INCLUDE} )

# find_library(GLUT_LIBRARY NAMES freeglut.lib freeglut.so freeglut.a libglut.so libglut.a )

if( NOT WIN32 )
    target_link_libraries( visualizer -lGL -lglut ${BOOST_PROGRAM_OPTIONS} )
else()
    add_definitions( -DFREEGLUT_STATIC )
    # target_link_libraries( visualizer ${PROJECT_LIB_NAME} ${GLUT_LIB} )
endif()

target_link_libraries( visualizer ${PROJECT_LIB_NAME} )

INSTALL ( FILES
          ${CMAKE_CURRENT_BINARY_DIR}/visualizer 

          PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_READ GROUP_EXECUTE
          DESTINATION ${CMAKE_INSTALL_PREFIX}
        )
