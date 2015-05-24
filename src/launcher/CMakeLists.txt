add_executable(moldynam_launcher moldynam_launcher.cpp)
target_link_libraries(moldynam_launcher moldynam_utils moldynam_native moldynam_opencl)
target_link_libraries(moldynam_launcher ${Boost_LIBRARIES})

INSTALL(TARGETS moldynam_launcher
	RUNTIME DESTINATION "bin"
)
