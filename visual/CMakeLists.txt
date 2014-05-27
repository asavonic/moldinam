cmake_minimum_required(VERSION 2.8.7)
set(PROJECT_NAME_STR visualizer)
project(${PROJECT_NAME_STR} C CXX)


add_executable(visualizer visualizer.cpp)

include_directories( glm )
if( NOT WIN32 )
    target_link_libraries( visualizer ${PROJECT_LIB_NAME} ${BOOST_PROGRAM_OPTIONS} -lGL -lglut )
else()
    target_link_libraries( visualizer ${PROJECT_LIB_NAME} ${GLUT_LIB} )
endif()

#installation of executable files
INSTALL ( FILES
          ${CMAKE_CURRENT_BINARY_DIR}/visualizer 

          PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_READ GROUP_EXECUTE
          DESTINATION ${CMAKE_INSTALL_PREFIX}
        )
