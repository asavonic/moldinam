#include <iostream>
#include <fstream>
#include <string>
#include <md_types.h>

std::vector<Molecule> read_molecules_from_file( std::string filepath );
void write_molecules_to_file( std::vector<Molecule>& molecules, std::string filepath, std::ios::openmode mode = std::ios::trunc );
