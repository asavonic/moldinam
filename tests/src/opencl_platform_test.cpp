#include "gtest/gtest.h"

#include <platforms/opencl/opencl_platform.hpp>
#include <platforms/opencl/opencl_dispatcher.hpp>
#include <platforms/opencl/opencl_helpers.hpp>

TEST(opencl_platform, system)
{
    OpenCLParticleSystem sys;
}

TEST(opencl_platform, hello_world_kernel)
{
    HelloWorldKernel kernel;
    kernel.execute();
}

TEST(opencl_platform, cl_to_native_conversion_simple)
{
    md::float3vec native_vec = { float3(0,0,0), float3(1,1,1), float3(2, 2, 3) };
    cl::float3vec cl_vec(native_vec.begin(), native_vec.end());

    md::float3vec converted = cl_vec.to_native();

    ASSERT_EQ(native_vec.size(), converted.size());

    for (size_t i = 0; i < native_vec.size(); i++) {
        EXPECT_EQ(native_vec[i], converted[i]);
    }
}
