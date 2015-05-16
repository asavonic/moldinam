#include "gtest/gtest.h"

#include <platforms/native/native_platform.hpp>

#include <md_types.h>
#include <md_algorithms.h>

#include "utils.hpp"

TEST(native_platform, system)
{
    NativeParticleSystem sys;
}

TEST(native_platform, verlet_reference_simple)
{
    Molecule mol;
    mol.pos = double3(0, 1, 1);
    mol.pos_prev = double3(0, 0.7, 0.3);

    ParticleSystemConfig conf;
    conf.dt = 0.5;

    NativeParticleSystem native(conf);

    float3vec pos { float3(0, 1, 1) };
    float3vec pos_prev { float3(0, 0.7, 0.3) };

    float3vec vel(1, float3(0, 0, 0));
    float3vec accel(1, float3(0, 0, 0));

    native.loadParticles(std::move(pos), std::move(pos_prev),
                         std::move(vel), std::move(accel));

    verlet(mol, conf.dt);

    native.applyVerletIntegration();

    EXPECT_FLOAT_EQ(mol.pos.x, native.pos()[0].x);
    EXPECT_FLOAT_EQ(mol.pos.y, native.pos()[0].y);
    EXPECT_FLOAT_EQ(mol.pos.z, native.pos()[0].z);

    EXPECT_FLOAT_EQ(mol.pos_prev.x, native.pos_prev()[0].x);
    EXPECT_FLOAT_EQ(mol.pos_prev.y, native.pos_prev()[0].y);
    EXPECT_FLOAT_EQ(mol.pos_prev.z, native.pos_prev()[0].z);
}

TEST(native_platform, verlet_reference_bruteforce)
{
    size_t num = 1024 * 1024;
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

