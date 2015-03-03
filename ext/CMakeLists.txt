set(EXT_DIR ${CMAKE_CURRENT_LIST_DIR})

add_subdirectory(gtest-1.7.0)
set(GTEST_INCLUDE_DIR ${EXT_DIR}/gtest-1.7.0/include CACHE STRING "Include directory for Google Test")

add_subdirectory(boost)
set(BOOST_INCLUDE_DIR ${EXT_DIR}/boost CACHE STRING "Include directory for boost")

add_subdirectory(glfw)
set(GLFW_INCLUDE_DIR ${EXT_DIR}/glfw/include CACHE STRING "Include directory for GLFW")

add_subdirectory(glm)
set(GLM_INCLUDE_DIR ${EXT_DIR}/glm CACHE STRING "Include directory for GLM")
