#include "gtest/gtest.h"

#include <platforms/native/native_platform.hpp>

#include <md_types.h>
#include <md_algorithms.h>

#include "utils.hpp"

TEST(native_platform, system)
{
    NativeParticleSystem sys;
}

void verlet_reference_bruteforce(size_t num)
{
    std::vector<Molecule> ref_mol = generate_random_molecules_vector(num);

    ParticleSystemConfig conf;
    conf.dt = 0.5;

    NativeParticleSystem native = md::legacy::convertToNativeSystem(ref_mol, conf);

    for (size_t i = 0; i < num; i++) {
        verlet(ref_mol[i], conf.dt);
    }

    native.applyVerletIntegration();

    for (size_t i = 0; i < num; i++) {
        ASSERT_FLOAT_EQ(ref_mol[i].pos.x, native.pos()[i].x) << "on step " << i;
        ASSERT_FLOAT_EQ(ref_mol[i].pos.y, native.pos()[i].y) << "on step " << i;
        ASSERT_FLOAT_EQ(ref_mol[i].pos.z, native.pos()[i].z) << "on step " << i;

        ASSERT_FLOAT_EQ(ref_mol[i].pos_prev.x, native.pos_prev()[i].x) << "on step " << i;
        ASSERT_FLOAT_EQ(ref_mol[i].pos_prev.y, native.pos_prev()[i].y) << "on step " << i;
        ASSERT_FLOAT_EQ(ref_mol[i].pos_prev.z, native.pos_prev()[i].z) << "on step " << i;
    }
}

TEST(native_platform, verlet_reference_bruteforce_light)
{
    verlet_reference_bruteforce(1024);
}

TEST(native_platform, verlet_reference_bruteforce_normal)
{
    verlet_reference_bruteforce(32 * 1024);
}

TEST(native_platform, verlet_reference_bruteforce_heavy)
{
    verlet_reference_bruteforce(1024 * 1024);
}

void euler_reference_bruteforce(size_t num)
{
    std::vector<Molecule> ref_mol = generate_random_molecules_vector(num);

    ParticleSystemConfig conf;
    conf.dt = 0.5;

    NativeParticleSystem native = md::legacy::convertToNativeSystem(ref_mol, conf);

    for (size_t i = 0; i < num; i++) {
        euler(ref_mol[i], conf.dt);
    }

    native.applyEulerIntegration();

    for (size_t i = 0; i < num; i++) {
        ASSERT_FLOAT_EQ(ref_mol[i].pos.x, native.pos()[i].x) << "on step " << i;
        ASSERT_FLOAT_EQ(ref_mol[i].pos.y, native.pos()[i].y) << "on step " << i;
        ASSERT_FLOAT_EQ(ref_mol[i].pos.z, native.pos()[i].z) << "on step " << i;

        ASSERT_FLOAT_EQ(ref_mol[i].pos_prev.x, native.pos_prev()[i].x) << "on step " << i;
        ASSERT_FLOAT_EQ(ref_mol[i].pos_prev.y, native.pos_prev()[i].y) << "on step " << i;
        ASSERT_FLOAT_EQ(ref_mol[i].pos_prev.z, native.pos_prev()[i].z) << "on step " << i;
    }
}

TEST(native_platform, euler_reference_bruteforce_light)
{
    euler_reference_bruteforce(1024);
}

TEST(native_platform, euler_reference_bruteforce_normal)
{
    euler_reference_bruteforce(32 * 1024);
}

TEST(native_platform, euler_reference_bruteforce_heavy)
{
    euler_reference_bruteforce(1024 * 1024);
}

