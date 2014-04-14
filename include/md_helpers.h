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

#endif
