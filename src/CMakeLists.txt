include_directories(${OPENCL_INCLUDE_DIRS})
include_directories(${MD_ROOT}/ext/glm)
include_directories(include)


set(MD_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR}/include CACHE STRING "Moldynam headers locations")

add_subdirectory(platforms)
add_subdirectory(utils)
add_subdirectory(lib)
add_subdirectory(basic)
add_subdirectory(visual)
add_subdirectory(launcher)
