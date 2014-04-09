#include <md_helpers.h>

std::vector<Molecule> read_molecules_from_file( std::string filepath ) {
    std::ifstream file( filepath.c_str(), std::ifstream::in );

    if ( !file.is_open() ) {
        throw std::runtime_error( "Error while opening file " + filepath + " for reading! Check if it exist and have correct permissions." );
    }

    std::string line;

    int lines_num = 0;
    file >> lines_num;

    std::vector<Molecule> molecules;
    molecules.reserve( lines_num );

    for ( int i = 0; i < lines_num; i++ ) {
        Molecule mol;
        file >> mol.pos.x >> mol.pos.y >> mol.pos.z >> mol.speed.x >> mol.speed.y >> mol.speed.z ;
        molecules.push_back( mol );
    }

    return molecules;
}

void write_molecules_to_file( std::vector<Molecule>& molecules, std::string filepath, std::ios::openmode mode ) {
    std::ofstream file( filepath.c_str(), std::ifstream::out | mode );

    if ( !file.is_open() ) {
        throw std::runtime_error( "Error while opening file " + filepath + " for writing! Check if it exist and have correct permissions." );
    }

    file << molecules.size() << std::endl;

    for ( unsigned int i = 0; i < molecules.size(); i++ ) {
        file << molecules[i].pos.x << " " << molecules[i].pos.y << " " << molecules[i].pos.z << " " 
             << molecules[i].speed.x << " " << molecules[i].speed.y << " " << molecules[i].speed.z << std::endl;
    }
}
