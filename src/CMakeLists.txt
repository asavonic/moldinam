include_directories(include)

set(MD_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR}/include CACHE STRING "Moldynam headers locations")

add_subdirectory(lib)
add_subdirectory(basic)
add_subdirectory(visual)
