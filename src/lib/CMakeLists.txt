set_property(GLOBAL PROPERTY PROJECT_LIB_NAME moldynam)

get_property(LIB_NAME GLOBAL PROPERTY PROJECT_LIB_NAME)

file(GLOB SRC_FILES *.cpp)

if (WIN32)
	add_definitions(-Dmoldinam_EXPORTS)
endif()

add_library(${LIB_NAME} SHARED ${SRC_FILES})

INSTALL(TARGETS ${LIB_NAME}
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)
