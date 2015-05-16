include_directories(${OPENCL_INCLUDE_DIRS})
add_library(moldynam_opencl
  opencl_platform.cpp
  kernels.cpp
)