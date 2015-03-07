#include <random>
#include <functional>
#include "gtest/gtest.h"
#include <md_algorithms.h>
#include <md_helpers.h>

void gold_Lennard_Jones( double r, double epsilon, double sigma, double& force, double& potential ) {
    double ri = 1 / r;
    double ri3 = ri * ri * ri;
    double ri6 = ri3 * ri3;
    
    force = 48 * epsilon * ( pow( sigma, 12 ) * ri6 - pow( sigma, 6 ) / 2 ) * ri6 * ri * ri;
    potential = 4 * epsilon * ri6 * ( ri6 * pow( sigma, 12 ) - pow( sigma, 6 ) );
}

TEST( algorithms, lennard_jones ) {
    std::random_device seed;
    std::mt19937 rand_engine( seed() ); 
    std::uniform_real_distribution<double> dist( 0.0, 300.0 );
    auto random_range = std::bind( dist, std::ref( rand_engine ) );

    double precision = 0.0000000001;

    for ( unsigned int i = 0; i < 1000000; i++ ) {
        double force = 0, force_gold = 0;
        double potential = 0, potential_gold = 0;

        LennardJonesConstants constants;
        constants.set_sigma( 1 );
        constants.set_eps( 1 );

        double r = random_range();

        gold_Lennard_Jones( r, constants.get_eps(), constants.get_sigma(), force_gold, potential_gold );
        Lennard_Jones( r, constants, force, potential );

        ASSERT_TRUE( std::abs( ( force_gold - force ) / force ) < precision );
        ASSERT_TRUE( std::abs( ( potential_gold - potential ) / potential ) < precision );
    }
}

TEST( algorithms, periodic_basic ) {
    double3 area_size;
    area_size.x = 3.0;
    area_size.y = 3.0;
    area_size.z = 3.0;

    Molecule mol_1;
    mol_1.pos.x = 5.0;
    mol_1.pos.y = 6.0;
    mol_1.pos.z = 7.0;

    periodic( mol_1, area_size );

    EXPECT_EQ( 2.0, mol_1.pos.x );
    EXPECT_EQ( 0.0, mol_1.pos.y );
    EXPECT_EQ( 1.0, mol_1.pos.z );

    Molecule mol_2;
    mol_2.pos.x = -5.0;
    mol_2.pos.y = -6.0;
    mol_2.pos.z = -7.0;

    periodic( mol_2, area_size );

    EXPECT_EQ( 1.0, mol_2.pos.x );
    EXPECT_EQ( 0.0, mol_2.pos.y );
    EXPECT_EQ( 2.0, mol_2.pos.z );
}

TEST( algorithms, periodic_full ) {
    double3 area_size;
    area_size.x = 3.0;
    area_size.y = 3.2;
    area_size.z = 2.3;

    std::random_device seed;
    std::mt19937 rand_engine( seed() ); 
    std::uniform_real_distribution<double> dist( -10.0, 10.0 );
    auto rand_double = std::bind( dist, std::ref( rand_engine ) );

    for (size_t i = 0; i < 1000000; ++i) {
        Molecule mol;
        mol.pos.x = rand_double();
        mol.pos.y = rand_double();
        mol.pos.z = rand_double();

        mol.pos_prev.x = rand_double();
        mol.pos_prev.y = rand_double();
        mol.pos_prev.z = rand_double();

        periodic(mol, area_size);

        ASSERT_TRUE(mol.pos.x < area_size.x);
        ASSERT_TRUE(mol.pos.x >= 0);
        ASSERT_TRUE(mol.pos_prev.x < area_size.x);
        ASSERT_TRUE(mol.pos_prev.x >= 0);

        ASSERT_TRUE(mol.pos.y < area_size.y);
        ASSERT_TRUE(mol.pos.y >= 0);
        ASSERT_TRUE(mol.pos_prev.y < area_size.y);
        ASSERT_TRUE(mol.pos_prev.y >= 0);

        ASSERT_TRUE(mol.pos.z < area_size.z);
        ASSERT_TRUE(mol.pos.z >= 0);
        ASSERT_TRUE(mol.pos_prev.z < area_size.z);
        ASSERT_TRUE(mol.pos_prev.z >= 0);
    }
}

TEST(algorithms, distance)
{
    Molecule mol1 = generate_random_molecule();
    Molecule mol2 = generate_random_molecule();

    double r = distance(mol1, mol2);

    double dx = mol1.pos.x - mol2.pos.x;
    double dy = mol1.pos.y - mol2.pos.y;
    double dz = mol1.pos.z - mol2.pos.z;

    double r_ref = sqrt( dx*dx + dy*dy + dz*dz );

    ASSERT_EQ(r_ref, r);
}
