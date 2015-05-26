cmake_minimum_required(VERSION 2.8.7)
set(PROJECT_NAME_STR visualizer)
project(${PROJECT_NAME_STR} C CXX)

get_property(MD_LIB_NAME GLOBAL PROPERTY PROJECT_LIB_NAME)

include_directories(${GLM_INCLUDE_DIR})
include_directories(${GLFW_INCLUDE_DIR})
include_directories(${GLEW_INCLUDE_DIR})
include_directories(${GLEXT_INCLUDE_DIR})
include_directories(${Boost_INCLUDE_DIRS})

add_executable(visualizer visualizer.cpp cube_renderer.cpp particles_renderer.cpp renderer.cpp)

target_link_libraries(visualizer ${MD_LIB_NAME})
target_link_libraries(visualizer ${Boost_LIBRARIES})
target_link_libraries(visualizer glfw ${GLFW_LIBRARIES} ${GLEW_LIBRARIES})

INSTALL(TARGETS visualizer visualizer_legacy
	RUNTIME DESTINATION "bin"
)
