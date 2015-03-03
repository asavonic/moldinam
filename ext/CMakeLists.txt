set(EXT_DIR ${CMAKE_CURRENT_LIST_DIR})

add_subdirectory(gtest-1.7.0)
set(GTEST_INCLUDE_DIR ${EXT_DIR}/gtest-1.7.0/include PARENT_SCOPE)

add_subdirectory(glfw)
set(GLFW_INCLUDE_DIR ${EXT_DIR}/glfw/include PARENT_SCOPE)

set(GLM_INCLUDE_DIR ${EXT_DIR}/glm PARENT_SCOPE)
