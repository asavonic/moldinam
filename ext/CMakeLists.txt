set(EXT_DIR ${CMAKE_CURRENT_LIST_DIR})

####
# Google's framework for writing C++ tests
####
add_subdirectory(gtest-1.7.0)
set(GTEST_INCLUDE_DIR ${EXT_DIR}/gtest-1.7.0/include PARENT_SCOPE)

####
# GLFW is a modern library for creating windows with OpenGL contexts and receiving input and events.
####
set(GLFW_BUILD_EXAMPLES OFF)
set(GLFW_BUILD_TESTS OFF)
set(GLFW_BUILD_DOCS OFF)
if (USE_STATIC_CRT)
    set(USE_MSVC_RUNTIME_LIBRARY_DLL OFF)
endif()
add_subdirectory(glfw)
set(GLFW_INCLUDE_DIR ${EXT_DIR}/glfw/include PARENT_SCOPE)

####
# GLM is a header only C++ mathematics library for graphics software
####
set(GLM_INCLUDE_DIR ${EXT_DIR}/glm PARENT_SCOPE)

####
# GL/glext.h - OpenGL 1.2 and above compatibility profile and extension interfaces. 
####
list(APPEND CMAKE_PREFIX_PATH ${EXT_DIR}/glext)
find_path(GLEXT_INCLUDE_DIR GL/glext.h)
if (NOT GLEXT_INCLUDE_DIR)
    message("GLEXT not found, downloading it from Khronos registry ...")
    file(MAKE_DIRECTORY ${EXT_DIR}/glext/GL)
    file(DOWNLOAD "https://www.opengl.org/registry/api/GL/glext.h" ${EXT_DIR}/glext/GL/glext.h )
endif()
find_path(GLEXT_INCLUDE_DIR GL/glext.h)

if (WIN32)
    set(GLEW_DIR ${EXT_DIR}/glew-1.12.0) 
    set(GLEW_LIB_DIR ${GLEW_DIR}/lib/Release/Win32) 

    list(APPEND CMAKE_PREFIX_PATH ${GLEW_DIR})
    list(APPEND CMAKE_LIBRARY_PATH ${GLEW_LIB_DIR})
endif()

####
# GLEW is a cross-platform open-source C/C++ extension loading library.
####
if (WIN32)
    set(GLEW_DIR "${EXT_DIR}/glew-1.12.0") 
    set(GLEW_LIB_DIR "${GLEW_DIR}/bin/Release/Win32") 

    list(APPEND CMAKE_PREFIX_PATH "${GLEW_DIR}")
    list(APPEND CMAKE_LIBRARY_PATH "${GLEW_LIB_DIR}")

    find_package(GLEW)
else()
    find_package(GLEW REQUIRED)
endif()
  
set(GLEW_LIBRARIES "${GLEW_LIBRARIES}" CACHE INTERNAL "GLEW libraries")

if (NOT ${GLEW_FOUND} AND WIN32)
    message("GLEW not found")

    set(GLEW_ZIP ${EXT_DIR}/glew-1.12.0.zip)

    if (NOT EXISTS ${GLEW_ZIP})
        message("Trying to download GLEW from sourceforge.net ...")
        file(DOWNLOAD http://downloads.sourceforge.net/project/glew/glew/1.12.0/glew-1.12.0-win32.zip?r=http%3A%2F%2Fsourceforge.net%2Fprojects%2Fglew%2Ffiles%2Fglew%2F1.12.0%2F&ts=1427032649&use_mirror=switch
                    ${EXT_DIR}/glew-1.12.0.zip)
    endif()

    message("We downloaded glew-1.12.0 for you, but since Windows")
    message("does not provide any standard tool for unzip")
    message("you need manually unzip ${GLEW_ZIP} into ${EXT_DIR}")
    message(FATAL_ERROR "Exiting now")
endif()

if (WIN32)
    find_file(GLEW_DLL "glew32.dll")
    INSTALL(FILES ${GLEW_DLL}
            DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
endif()
