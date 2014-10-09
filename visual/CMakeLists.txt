cmake_minimum_required(VERSION 2.8.7)
set(PROJECT_NAME_STR visualizer)
project(${PROJECT_NAME_STR} C CXX)


add_executable(visualizer visualizer.cpp particles_renderer.cpp )

include_directories( ${GLUT_INCLUDE} )


if( NOT WIN32 )
    target_link_libraries( visualizer -lGL -lGLU -lGLEW  glfw ${GLFW_LIBRARIES} ${BOOST_PROGRAM_OPTIONS} )
else()
    add_definitions( -DFREEGLUT_STATIC )
endif()

target_link_libraries( visualizer ${PROJECT_LIB_NAME} )

INSTALL ( FILES
          ${CMAKE_CURRENT_BINARY_DIR}/visualizer 

          PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_READ GROUP_EXECUTE
          DESTINATION ${CMAKE_INSTALL_PREFIX}
        )
