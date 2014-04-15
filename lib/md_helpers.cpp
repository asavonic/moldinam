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
        file >> mol;
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
        file << molecules[i] << std::endl;
    }
}

std::ostream& operator<<( std::ostream& os, const Molecule molecule ) {
    os << molecule.pos.x << " " << molecule.pos.y << " " << molecule.pos.z << " " 
         << molecule.speed.x << " " << molecule.speed.y << " " << molecule.speed.z;

    return os;
}

std::istream& operator>>( std::istream& is,  Molecule& molecule ) {
    is >> molecule.pos.x >> molecule.pos.y >> molecule.pos.z >> molecule.speed.x >> molecule.speed.y >> molecule.speed.z ;
    return is;
}


trace_read::trace_read() {
    molecules_num = 0;
}

trace_read::trace_read( std::string filepath ) {
    molecules_num = 0;
    this->open( filepath );
}

void trace_read::open( std::string filepath ) {
    file.open( filepath.c_str(), std::ifstream::in );
    if ( !file.is_open() ) {
        throw std::runtime_error( "Error while opening file " + filepath + " for reading! Check if it exist and have correct permissions." );
    }

}

std::vector<Molecule> trace_read::initial() {
    if ( !file.is_open() ) {
        throw std::logic_error( std::string( "Cannot read file which is not opened at " ) + std::string( __FILE__ ) + ":" + std::to_string( __LINE__ ) );
    } 

    file.seekg( 0, file.beg );
    file >> molecules_num;

    std::vector<Molecule> molecules( molecules_num );

    for ( size_t i = 0; i < molecules_num; i++ ) {
        file >> molecules[i];
    }

    return std::move( molecules );
}

std::vector<Molecule> trace_read::next() {
    if ( !file.is_open() ) {
        throw std::logic_error( std::string( "Cannot read file which is not opened at " ) + std::string( __FILE__ ) + ":" + std::to_string( __LINE__ ) );
    } 

    if ( file.tellg() == file.beg ) {
        return std::move( this->initial() );
    }
    else {
        std::vector<Molecule> molecules( molecules_num );

        for ( size_t i = 0; i < molecules_num; i++ ) {
            file >> molecules[i];
        }

        return std::move( molecules );
    }
}

std::vector<Molecule> trace_read::final() {
    if ( !file.is_open() ) {
        throw std::logic_error( std::string( "Cannot read file which is not opened at " ) + std::string( __FILE__ ) + ":" + std::to_string( __LINE__ ) );
    } 
    
    file.seekg( file.end );
    size_t length = file.tellg();

    size_t lines_end_num = 0; // counting lines from the end of the file
                              // once we reach the beginning of last group -- read it
                              
    for(int i = length-2; i > 0 || lines_end_num < molecules_num ; i-- ) { 
        file.seekg(i);
        char c = file.get();
        if( c == '\r' || c == '\n' ) { //new line?
            lines_end_num ++;
        }
    }

    std::vector<Molecule> molecules( molecules_num );

    for ( size_t i = 0; i < molecules_num; i++ ) {
        file >> molecules[i];
    }

    return std::move( molecules );
}
