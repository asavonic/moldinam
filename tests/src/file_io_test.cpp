#include <md_helpers.h>
#include "gtest/gtest.h"


std::vector<Molecule> read_molecules_from_file( std::string filepath );

TEST( file_io, state_read ) {
    std::vector<Molecule>  molecules = read_molecules_from_file( "test_file_io_read_from_file.xyz" ); 

    unsigned int required_size = 5;
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


TEST( file_io, state_write ) {
    std::vector<Molecule>  molecules;
    molecules.resize( 5 );

    for ( unsigned int i = 0; i < molecules.size(); i++ ) {
         molecules[i].pos.x = i + 0.1;
         molecules[i].pos.y = i + 0.2;
         molecules[i].pos.z = i + 0.3;

         molecules[i].speed.x = i + 0.4;
         molecules[i].speed.y = i + 0.5;
         molecules[i].speed.z = i + 0.6;
    }

    write_molecules_to_file( molecules, "test_file_io_write_to_file.xyz" );
}

bool check_mol_vectors_equal( std::vector<Molecule>& first, std::vector<Molecule>& second ) {
    size_t first_size = first.size();
    size_t second_size = second.size();
    if ( first_size != second_size ) {
        return false;
    }

    double tol = 1 / ( 1 << 20 );
    for ( size_t i = 0; i < first.size(); i++ ) {
        if ( std::abs( first[i].pos.x - second[i].pos.x ) > tol ) {
            return false;
        }
        if ( std::abs( first[i].pos.y - second[i].pos.y ) > tol ) {
            return false;
        }
        if ( std::abs( first[i].pos.z - second[i].pos.z ) > tol ) {
            return false;
        }
        if ( std::abs( first[i].speed.x - second[i].speed.x ) > tol ) {
            return false;
        }
        if ( std::abs( first[i].speed.y - second[i].speed.y ) > tol ) {
            return false;
        }
        if ( std::abs( first[i].speed.z - second[i].speed.z ) > tol ) {
            return false;
        }
        if ( std::abs( first[i].accel.x - second[i].accel.x ) > tol ) {
            return false;
        }
        if ( std::abs( first[i].accel.y - second[i].accel.y ) > tol ) {
            return false;
        }
        if ( std::abs( first[i].accel.z - second[i].accel.z ) > tol ) {
            return false;
        }
        if ( first[i].type != second[i].type ) {
            return false;
        }
    }

    return true;
}

TEST( file_io, trace_read_write ) {
    auto molecules_initial_reference = generate_random_molecules_vector( 10 );
    auto molecules_step1_reference   = generate_random_molecules_vector( 10 );
    auto molecules_step2_reference   = generate_random_molecules_vector( 10 );
    auto molecules_step3_reference   = generate_random_molecules_vector( 10 );
    auto molecules_step4_reference   = generate_random_molecules_vector( 10 );
    auto molecules_final_reference   = generate_random_molecules_vector( 10 );

    trace_write _trace_write( "trace_test.xyztrace" );

    _trace_write.initial( molecules_initial_reference );
    _trace_write.next( molecules_step1_reference );
    _trace_write.next( molecules_step2_reference );
    _trace_write.next( molecules_step3_reference );
    _trace_write.next( molecules_step4_reference );
    _trace_write.next( molecules_final_reference );

    trace_read _trace_read( "trace_test.xyztrace" );
    auto molecules_initial = _trace_read.initial();
    auto molecules_step1   = _trace_read.next();
    auto molecules_step2   = _trace_read.next();
    auto molecules_step3   = _trace_read.next();
    auto molecules_step4   = _trace_read.next();
    auto molecules_final   = _trace_read.next();

    trace_read _trace_read_2 ( "trace_test.xyztrace" );
    auto molecules_final_2 = _trace_read_2.final();
    
    check_mol_vectors_equal( molecules_initial_reference, molecules_initial );
    check_mol_vectors_equal( molecules_step1_reference, molecules_step1 );
    check_mol_vectors_equal( molecules_step2_reference, molecules_step2 );
    check_mol_vectors_equal( molecules_step3_reference, molecules_step3 );
    check_mol_vectors_equal( molecules_step4_reference, molecules_step4 );
    check_mol_vectors_equal( molecules_final_reference, molecules_final );
    check_mol_vectors_equal( molecules_final, molecules_final_2 );
}
