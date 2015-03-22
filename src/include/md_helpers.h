#ifndef __MD_HELPERS_H
#define __MD_HELPERS_H

#include <iostream>
#include <fstream>
#include <string>
#include <md_types.h>
#include <stdexcept>
#include <random>
#include <functional>
#include <map>

#include "dll_defines.h"

MOLDINAM_EXPORT std::vector<Molecule> read_molecules_from_file( std::string filepath );
MOLDINAM_EXPORT void write_molecules_to_file( std::vector<Molecule>& molecules, std::string filepath, std::ios::openmode mode = std::ios::trunc );
MOLDINAM_EXPORT std::ostream& operator<<( std::ostream&, const Molecule );
MOLDINAM_EXPORT std::istream& operator>>( std::istream&,  Molecule& );

MOLDINAM_EXPORT std::ostream& operator<<( std::ostream&, const Molecule_Type );
MOLDINAM_EXPORT std::istream& operator>>( std::istream&,  Molecule_Type& );

class MOLDINAM_EXPORT trace_read {
public:

    bool active; // TODO trace_read becomes deactivated when reaches the end of trace file
                 // this is temporary solution, must be fixed later   
    trace_read();
    trace_read( std::string filepath );
    void open( std::string filepath );

    // reset file to beginning and read initial molecules state
    std::vector<Molecule> initial();

    std::vector<Molecule> next();
    
    // read final state of molecules, reset file to the end
    std::vector<Molecule> final();

    void close();

private:

    size_t read_total_steps();

    std::ifstream file;
    size_t molecules_num;
    size_t steps;
    size_t total_steps;
};

class MOLDINAM_EXPORT trace_write {
public:
    // setting active to false will disable all functionality
    bool active;

    trace_write();
    trace_write( std::string filepath );
    void open( std::string filepath );

    void initial( std::vector<Molecule>& );
    void next( std::vector<Molecule>& );
    void final( std::vector<Molecule>& );

    void close();

private:
    std::ofstream file;
    size_t steps;
};

MOLDINAM_EXPORT Molecule generate_random_molecule();
MOLDINAM_EXPORT std::vector<Molecule> generate_random_molecules_vector( size_t );


class MOLDINAM_EXPORT LennardJonesConstants 
{
    public:

    LennardJonesConstants() {
        sigma = sigma_pow_6 = sigma_pow_12 = 1;
        eps = eps_pow_6 = eps_pow_12 = 1;
    }

    void set_sigma( double _sigma ) {
        sigma = _sigma;
        sigma_pow_6 = std::pow( sigma, 6 );
        sigma_pow_12 = std::pow( sigma, 12 );
    }

    void set_eps( double _eps ) {
        eps = _eps;
        eps_pow_6 = std::pow( eps, 6 );
        eps_pow_12 = std::pow( eps, 12 );
    }

    double get_sigma() { return sigma; }
    double get_eps() { return eps; }
    double get_sigma_pow_6() { return sigma_pow_6; }
    double get_sigma_pow_12() { return sigma_pow_12; }

    private:

    double sigma;
    double eps;

    // precomputed pow( sigma, N ) used due to performance reasons
    double sigma_pow_6;
    double eps_pow_6;

    double sigma_pow_12;
    double eps_pow_12;
};

// TODO tests missing
class MOLDINAM_EXPORT LennardJonesConfig 
{
    public:

    LennardJonesConfig( std::string );
    LennardJonesConstants get_constants( Molecule_Type type ) { return type_constants_map[ type ]; }

    private:

    std::map<Molecule_Type, LennardJonesConstants> type_constants_map;
};
#endif
