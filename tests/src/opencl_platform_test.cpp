#include "gtest/gtest.h"

#include <platforms/opencl/opencl_platform.hpp>
#include <platforms/opencl/opencl_dispatcher.hpp>

TEST(opencl_platform, system)
{
    OpenCLParticleSystem sys;
}

TEST(opencl_platform, hello_world_kernel)
{
    HelloWorldKernel kernel;
    kernel.execute();
}
