include_directories(${OPENCL_INCLUDE_DIRS})
include_directories(${Boost_INCLUDE_DIRS})

add_executable(moldynam_launcher moldynam_launcher.cpp)
target_link_libraries(moldynam_launcher moldynam_utils moldynam_native moldynam_opencl moldynam_tbb tbb)
target_link_libraries(moldynam_launcher ${OPENCL_LIBRARIES}) # TODO: remove linkage and replace by dll load?

target_link_libraries(moldynam_launcher ${Boost_LIBRARIES})

INSTALL(TARGETS moldynam_launcher
	RUNTIME DESTINATION "bin"
)
