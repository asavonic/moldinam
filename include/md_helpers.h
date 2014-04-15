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

    // reset file to beginning and read initial molecules state
    std::vector<Molecule> initial();

    std::vector<Molecule> next();
    
    // read final state of molecules, reset file to the end
    std::vector<Molecule> final();

private:
    std::ifstream file;
    size_t molecules_num;
};

class trace_write {
public:
    // setting active to false will disable all functionality. use with caution.
    bool active;

    trace_write();
    trace_write( std::string filepath );
    void open( std::string filepath );

    void initial( std::vector<Molecule>& );
    void next( std::vector<Molecule>& );

private:
    std::ofstream file;
};

#endif
