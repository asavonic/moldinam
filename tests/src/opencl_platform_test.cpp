#include "gtest/gtest.h"

#include <platforms/opencl/opencl_platform.hpp>
#include <platforms/opencl/opencl_dispatcher.hpp>
#include <platforms/opencl/opencl_helpers.hpp>

#include <md_types.h>
#include <md_algorithms.h>

#include "utils.hpp"

TEST(opencl_platform, system)
{
    OpenCLParticleSystem sys;
}

TEST(opencl_platform, hello_world_kernel)
{
    HelloWorldKernel kernel;
    kernel.execute();
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

void verlet_reference_bruteforce(size_t num)
{
    std::vector<Molecule> ref_mol = generate_random_molecules_vector(num);

    ParticleSystemConfig conf;
    conf.dt = 0.5;

    NativeParticleSystem native = md::legacy::convertToNativeSystem(ref_mol, conf);
    OpenCLParticleSystem cl_sys;
    cl_sys.fromNative(native);

    for (size_t i = 0; i < num; i++) {
        verlet(ref_mol[i], conf.dt);
    }

    cl_sys.applyVerletIntegration();

    NativeParticleSystem converted_native = cl_sys.convertToNative();

    for (size_t i = 0; i < num; i++) {
        ASSERT_FLOAT_EQ(ref_mol[i].pos.x, converted_native.pos()[i].x) << "on step " << i;
        ASSERT_FLOAT_EQ(ref_mol[i].pos.y, converted_native.pos()[i].y) << "on step " << i;
        ASSERT_FLOAT_EQ(ref_mol[i].pos.z, converted_native.pos()[i].z) << "on step " << i;

        ASSERT_FLOAT_EQ(ref_mol[i].pos_prev.x, converted_native.pos_prev()[i].x) << "on step " << i;
        ASSERT_FLOAT_EQ(ref_mol[i].pos_prev.y, converted_native.pos_prev()[i].y) << "on step " << i;
        ASSERT_FLOAT_EQ(ref_mol[i].pos_prev.z, converted_native.pos_prev()[i].z) << "on step " << i;
    }
}

TEST(opencl_platform, verlet_reference_bruteforce_light)
{
    verlet_reference_bruteforce(1024);
}

TEST(opencl_platform, verlet_reference_bruteforce_normal)
{
    verlet_reference_bruteforce(32 * 1024);
}

TEST(opencl_platform, verlet_reference_bruteforce_heavy)
{
    verlet_reference_bruteforce(1024 * 1024);
}


void euler_reference_bruteforce(size_t num)
{
    std::vector<Molecule> ref_mol = generate_random_molecules_vector(num);

    ParticleSystemConfig conf;
    conf.dt = 0.5;

    NativeParticleSystem native = md::legacy::convertToNativeSystem(ref_mol, conf);
    OpenCLParticleSystem cl_sys;
    cl_sys.fromNative(native);

    for (size_t i = 0; i < num; i++) {
        euler(ref_mol[i], conf.dt);
    }

    cl_sys.applyEulerIntegration();

    NativeParticleSystem converted_native = cl_sys.convertToNative();

    for (size_t i = 0; i < num; i++) {
        ASSERT_FLOAT_EQ(ref_mol[i].pos.x, converted_native.pos()[i].x) << "on step " << i;
        ASSERT_FLOAT_EQ(ref_mol[i].pos.y, converted_native.pos()[i].y) << "on step " << i;
        ASSERT_FLOAT_EQ(ref_mol[i].pos.z, converted_native.pos()[i].z) << "on step " << i;

        ASSERT_FLOAT_EQ(ref_mol[i].pos_prev.x, converted_native.pos_prev()[i].x) << "on step " << i;
        ASSERT_FLOAT_EQ(ref_mol[i].pos_prev.y, converted_native.pos_prev()[i].y) << "on step " << i;
        ASSERT_FLOAT_EQ(ref_mol[i].pos_prev.z, converted_native.pos_prev()[i].z) << "on step " << i;
    }
}

TEST(opencl_platform, euler_reference_bruteforce_light)
{
    euler_reference_bruteforce(1024);
}

TEST(opencl_platform, euler_reference_bruteforce_normal)
{
    euler_reference_bruteforce(32 * 1024);
}

TEST(opencl_platform, euler_reference_bruteforce_heavy)
{
    euler_reference_bruteforce(1024 * 1024);
}

TEST(opencl_platform, store)
{
    std::vector<Molecule> ref_mol = generate_random_molecules_vector(10);

    ParticleSystemConfig conf;
    conf.dt = 0.5;

    NativeParticleSystem native = md::legacy::convertToNativeSystem(ref_mol, conf);
    OpenCLParticleSystem cl_sys;
    cl_sys.fromNative(native);

    std::cout << "Native:\n";
    native.storeParticles(std::cout);
    std::cout << "OpenCL:\n";
    cl_sys.storeParticles(std::cout);
}
