#ifndef __MD_HELPERS_H
#define __MD_HELPERS_H

#include <iostream>
#include <fstream>
#include <string>
#include <md_types.h>
#include <stdexcept>

std::vector<Molecule> read_molecules_from_file( std::string filepath );
void write_molecules_to_file( std::vector<Molecule>& molecules, std::string filepath, std::ios::openmode mode = std::ios::trunc );
std::ostream& operator<<( std::ostream& os, const Molecule );
std::istream& operator>>( std::istream& is,  Molecule& );

class trace_read {
public:
    trace_read();
    trace_read( std::string filepath );
    void open( std::string filepath );

    std::vector<Molecule> initial();
    std::vector<Molecule> next();
    std::vector<Molecule> final();

private:
    std::ifstream file;
    size_t molecules_num;
};

class trace_write {
public:
    trace_write();
    trace_write( std::string filepath );
    void open( std::string filepath );

    // this will reset trace file to beginning
    void initial( std::vector<Molecule>& );
    void next( std::vector<Molecule>& );
    void final( std::vector<Molecule>& );

private:
    std::ofstream file;
    size_t molecules_num;
};

#endif
