include_directories(${OPENCL_INCLUDE_DIRS})
add_library(moldynam_opencl
  opencl_helpers.cpp
  opencl_platform.cpp
  kernels.cpp
)