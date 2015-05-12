include_directories(${OPENCL_INCLUDE_DIRS})
add_library(moldynam_opencl
  kernels.cpp
)