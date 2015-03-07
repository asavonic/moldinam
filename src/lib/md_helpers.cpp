#include <md_helpers.h>
#include <regex>
#include <sstream>

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
    os.precision( std::numeric_limits<double>::digits10 ); // need to avoid miscompare when reading and writing

    os << molecule.type << " "
         << molecule.pos.x << " " << molecule.pos.y << " " << molecule.pos.z << " " 
         << molecule.speed.x << " " << molecule.speed.y << " " << molecule.speed.z << " "
         << molecule.accel.x << " " << molecule.accel.y << " " << molecule.accel.z;

    return os;
}

std::istream& operator>>( std::istream& is,  Molecule& molecule ) {
    is.precision( std::numeric_limits<double>::digits10 ); // need to avoid miscompare when reading and writing

    is >> molecule.type;
    is >> molecule.pos.x >> molecule.pos.y >> molecule.pos.z;
    is >> molecule.speed.x >> molecule.speed.y >> molecule.speed.z;
    is >> molecule.accel.x >> molecule.accel.y >> molecule.accel.z;
    return is;
}


trace_read::trace_read() {
    molecules_num = 0;
    active = false;
}

trace_read::trace_read( std::string filepath ) {
    molecules_num = 0;
    this->open( filepath );
}

void trace_read::close()
{
    if ( file.is_open() ) {
        file.close();
    }
}

void trace_read::open( std::string filepath ) {
    if ( file.is_open() ) {
        file.close();
    }

    file.open( filepath.c_str(), std::ifstream::in );
    if ( !file.is_open() ) {
        throw std::runtime_error( "Error while opening file " + filepath + " for reading! Check if it exist and have correct permissions." );
    }
    
    active = true;
    file >> molecules_num;
    total_steps = read_total_steps();
}

size_t trace_read::read_total_steps()
{
    if ( !file.is_open() ) {
        throw std::logic_error(std::string( "Cannot read file which was not opened" ));
    } 

    size_t initial_pos = file.tellg();

    file.seekg( 0, file.end );
    size_t length = file.tellg();

    for(int i = length-2; i > 0 ; i-- ) { 
        file.seekg(i);
        char c = file.get();
        if( c == '\r' || c == '\n' ) { //new line?
            break;
        }
    }

    std::string total_steps_str;
    std::getline( file, total_steps_str );

    std::string substr = "total steps = ";
    size_t substr_pos = total_steps_str.find( substr );
    if ( substr_pos != std::string::npos ) {
        total_steps_str = total_steps_str.substr( substr_pos + substr.length() );
    }
    else {
        throw std::runtime_error( "Total steps number not found in trace file" );
    }

    file.seekg(initial_pos);
    return std::stoi( total_steps_str );
}

std::vector<Molecule> trace_read::initial() {

    file.seekg( 0, file.beg );
    file >> molecules_num;

    std::vector<Molecule> molecules( molecules_num );

    for ( size_t i = 0; i < molecules_num; i++ ) {
        file >> molecules[i];
    }

    steps++;

    return molecules;
}

std::vector<Molecule> trace_read::next() {
    if ( !file.is_open() ) {
        throw std::logic_error( std::string( "Cannot read file which is not opened at " ) + std::string( __FILE__ ) + ":" + std::to_string( __LINE__ ) );
    } 

    if ( static_cast<size_t>( file.tellg() ) == file.beg ) {
        return this->initial();
    }

    if ( total_steps == steps ) {
        std::cout << "trace_read reached end of the file" << std::endl;
        active = false;
        return std::vector<Molecule>();
    }
    else {
        std::vector<Molecule> molecules( molecules_num );

        for ( size_t i = 0; i < molecules_num; i++ ) {
            file >> molecules[i];
        }
        steps++;

        if ( total_steps == steps ) {
            std::cout << "trace_read reached end of the file" << std::endl;
            active = false;
        }

        return molecules;
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
                              
                                                /* total steps + empty line*/
    for(int i = length-2; i > 0 && lines_end_num < molecules_num + 2 ; i-- ) { 
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

    return molecules;
}


trace_write::trace_write() {
    active = true;
}

trace_write::trace_write( std::string filepath ) {
    active = true;
    this->open( filepath );
}

void trace_write::close()
{
    if ( file.is_open() ) {
        file.close();
    }
}

void trace_write::open( std::string filepath ) {
    if ( file.is_open() ) {
        file.close();
    }

    file.open( filepath.c_str(), std::ofstream::out );
    if ( !file.is_open() ) {
        throw std::runtime_error( "Error while opening file " + filepath + " for reading! Check if it exist and have correct permissions." );
    }
}

void trace_write::initial( std::vector<Molecule>& molecules ) {
    if ( this->active == false ) {
        return;
    }

    if ( !file.is_open() ) {
        throw std::runtime_error( "Cannot write to file: file is not opened" );
    }

    steps = 0;
    file << molecules.size() << std::endl;

    for ( size_t i = 0; i < molecules.size(); i++ ) {
        file << molecules[i] << std::endl;
    }

    file << std::endl;
    steps++;
}

void trace_write::next( std::vector<Molecule>& molecules ) {
    if ( this->active == false ) {
        return;
    }

    for ( size_t i = 0; i < molecules.size(); i++ ) {
        file << molecules[i] << std::endl;
    }

    file << std::endl;
    steps++;
}

void trace_write::final( std::vector<Molecule>& molecules ) {
    next( molecules );
    file << "total steps = " + std::to_string( steps ) << std::endl;
    active = false;
}

Molecule generate_random_molecule() {
    std::mt19937 rng_engine;
    std::uniform_real_distribution<double> dist( 0.0, 5.0 );

    auto rand_double = std::bind(dist, std::ref(rng_engine));

    Molecule mol;
    mol.pos.x = rand_double();
    mol.pos.y = rand_double();
    mol.pos.z = rand_double();

    mol.speed.x = rand_double();
    mol.speed.y = rand_double();
    mol.speed.z = rand_double();

    mol.accel.x = rand_double();
    mol.accel.y = rand_double();
    mol.accel.z = rand_double();

    mol.type = Molecule_Type::H; // chosen by fair dice roll
                                 // guaranted to be random

    return mol;
}
std::vector<Molecule> generate_random_molecules_vector( size_t size ) {
    std::vector<Molecule> molecules( size );
    for ( size_t i = 0; i < molecules.size(); i++ ) {
        molecules[i] = generate_random_molecule();
    }

    return molecules;
}


std::ostream& operator<<( std::ostream& os, const Molecule_Type type ) {
    switch ( type )  {
        case Molecule_Type::H : { os << "H"; break; }
        case Molecule_Type::O : { os << "O"; break; }

        case Molecule_Type::NO_TYPE : { os << "H"; break; }
        default : { throw std::runtime_error( "Unexpected molecule type value: " + std::to_string( static_cast<int>( type ) ) ); }
    }
    return os;
}

std::istream& operator>>( std::istream& is,  Molecule_Type& type ) {
    std::string str_type;
    is >> str_type;

    if ( str_type == "H" ) {
        type = Molecule_Type::H;
        return is;
    } 

    if ( str_type == "O" ) {
        type = Molecule_Type::O;
        return is;
    } 

    if ( str_type == "NO_TYPE" ) {
        type = Molecule_Type::NO_TYPE;
        return is;
    } 

    throw std::runtime_error( "Unsupported molecule type: " + str_type );
    return is;
}

LJ_config::LJ_config( std::string config_path ) {
    std::ifstream file( config_path.c_str(), std::ifstream::in );

    if ( !file.is_open() ) {
        throw std::runtime_error( "Error while opening file " + config_path + " for reading! Check if it exist and have correct permissions." );
    }

    molecule_types_constants.resize( 1000, std::make_pair(0,0) );
    std::cout << "Loading config..." << std::endl;
    Molecule_Type type = Molecule_Type::NO_TYPE;
    double sigma = 0;
    double eps   = 0;

    std::string line;
    while( std::getline(file, line) ) {
        std::istringstream iss(line);
        if( !(iss >> type >> sigma >> eps) ) {
            break; 
        } 
        molecule_types_constants[ static_cast<int>(type) ] = std::make_pair( sigma, eps );

        std::cout << type << " " << sigma << " " << eps << std::endl;
    }

    std::cout << "Loading complete" << std::endl;
    
}
