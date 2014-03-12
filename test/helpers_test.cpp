#include <md_helpers.h>
#include "gtest/gtest.h"


std::vector<Molecule> read_molecules_from_file( std::string filepath );

TEST( helpers, read_from_file ) {
    std::vector<Molecule>  molecules = read_molecules_from_file( "test_helpers_read_from_file.xyz" ); 

    unsigned int required_size = 10;
    ASSERT_EQ( required_size, molecules.size() );

    for ( int i = 0; i < 5; i++ ) {
        ASSERT_EQ( i + 0.1, molecules[i].pos.x );
        ASSERT_EQ( i + 0.2, molecules[i].pos.y );
        ASSERT_EQ( i + 0.3, molecules[i].pos.z );

        ASSERT_EQ( i + 0.4, molecules[i].speed.x );
        ASSERT_EQ( i + 0.5, molecules[i].speed.y );
        ASSERT_EQ( i + 0.6, molecules[i].speed.z );
    }
}
