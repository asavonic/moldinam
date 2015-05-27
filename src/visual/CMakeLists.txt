cmake_minimum_required(VERSION 2.8.7)
set(PROJECT_NAME_STR visualizer)
project(${PROJECT_NAME_STR} C CXX)

# include_directories(${GLM_INCLUDE_DIR})
# TODO: NOTE: BUG:
# GLM include commented out
# for some reason it causes linker error:
# visualizer.cpp:(.text._ZN11ConfigEntryIN3glm6detail5tvec3IfEEE19readValueFromStreamERSi[_ZN11ConfigEntryIN3glm6detail5tvec3IfEEE19readValueFromStreamERSi]+0xb): undefined reference to `md::operator>>(std::istream&, glm::detail::tvec3<float>&)'

# WTF?! md::openrator>> is a part of moldynam_native, this makes no sense
# CMake issue?

include_directories(${GLFW_INCLUDE_DIR})
include_directories(${GLEXT_INCLUDE_DIR})
include_directories(${Boost_INCLUDE_DIRS})

add_executable(visualizer visualizer.cpp cube_renderer.cpp particles_renderer.cpp renderer.cpp)

target_link_libraries(visualizer moldynam_utils moldynam_native)
target_link_libraries(visualizer ${Boost_LIBRARIES})
target_link_libraries(visualizer glfw ${GLFW_LIBRARIES} ${GLEW_LIBRARIES})

INSTALL(TARGETS visualizer
	RUNTIME DESTINATION "bin"
)
