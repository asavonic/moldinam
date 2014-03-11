#include <random>
#include <functional>
#include "gtest/gtest.h"
#include <md_algorithms.h>

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
        double sigma = 1;
        double epsilon = 1;

        double r = random_range();

        gold_Lennard_Jones( r, epsilon, sigma, force_gold, potential_gold );
        Lennard_Jones( r, epsilon, sigma, force, potential );

        ASSERT_TRUE( std::abs( ( force_gold - force ) / force ) < precision );
        ASSERT_TRUE( std::abs( ( potential_gold - potential ) / potential ) < precision );
    }

}
