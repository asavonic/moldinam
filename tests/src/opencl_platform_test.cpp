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

template <class T1, class T2>
void EXPECT_CONTAINERS_EQUAL(const T1& lhs, const T2& rhs)
{
    ASSERT_EQ(lhs.size(), rhs.size());

    for (size_t i = 0; i < lhs.size(); i++) {
        EXPECT_EQ(lhs[i], rhs[i]);
    }
}


TEST(opencl_platform, buffer_cl_to_native_conversion_simple)
{
    md::float3vec native_vec = { float3(0,0,0), float3(1,1,1), float3(2, 2, 3) };
    cl::float3vec cl_vec(native_vec.begin(), native_vec.end());

    md::float3vec converted = cl_vec.to_native();

    EXPECT_CONTAINERS_EQUAL(native_vec, converted);
}



TEST(opencl_platform, native_to_cl_system_conversion_simple)
{
    md::float3vec pos = { float3(0,0,0), float3(1,1,1), float3(2, 2, 3) };
    md::float3vec pos_prev = { float3(1,0,0), float3(1,1,1), float3(2, 2, 3) };
    md::float3vec vel = { float3(2,0,0), float3(1,1,1), float3(2, 2, 3) };
    md::float3vec accel = { float3(2,2,0), float3(1,1,1), float3(2, 2, 3) };

    NativeParticleSystem native;
    native.loadParticles(std::move(pos), std::move(pos_prev),
                         std::move(vel), std::move(accel));

    OpenCLParticleSystem cl_sys;

    cl_sys.fromNative(native);

    NativeParticleSystem converted_native = cl_sys.convertToNative();

    EXPECT_CONTAINERS_EQUAL(native.pos(), converted_native.pos());
    EXPECT_CONTAINERS_EQUAL(native.pos_prev(), converted_native.pos_prev());
    EXPECT_CONTAINERS_EQUAL(native.vel(), converted_native.vel());
    EXPECT_CONTAINERS_EQUAL(native.accel(), converted_native.accel());
}
