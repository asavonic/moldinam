#include <boost/program_options.hpp>
#include <iostream>
#include <vector> 
#include <string>
#include <md_algorithms.h>
#include <md_helpers.h>

namespace po = boost::program_options;

void moldinam_basic( std::string input_file_path, std::string output_file_path, size_t iterations, double dt, bool use_periodic, std::string use_trace );

int main( int argc, char** argv ) {
    try {
        int          iterations = 0;
        double       dt = 0;
        std::string  input_file_path;
        std::string  output_file_path;
        bool         use_periodic = false;
        std::string  trace_file;

        // named arguments
        po::options_description desc("Allowed options");
        desc.add_options()
            ("help", "produce help message")
            ("iterations", po::value<int>( &iterations )->required(), "number of iterations")
            ("dt", po::value<double>( &dt )->default_value( 0.00001 ), "dt on each step")
            ("input,i", po::value< std::string >( &input_file_path )->required(), "path to input .xyz file")
            ("output,o", po::value< std::string >( &output_file_path )->required(), "path to output .xyz file")
            ("periodic", po::value<bool>( &use_periodic ), "compute with XYZ periodic boundaries")
            ("trace", po::value< std::string >( &trace_file ), "save condition on each iteration to file")
        ;

        // positional arguments
        po::positional_options_description p;
        p.add("input", -1);

        po::variables_map vm;
        po::store( po::command_line_parser( argc, argv).options(desc).positional(p).run(), vm );

        if (vm.count("help")) {
            std::cout << desc << std::endl;
            return 1;
        }

        po::notify(vm);


        std::cout << "input file  = " << input_file_path << std::endl;
        std::cout << "output file = " << output_file_path << std::endl;
        std::cout << "iterations  = " << iterations << std::endl;   
        std::cout << "dt          = " << dt << std::endl;   

        moldinam_basic( input_file_path, output_file_path, iterations, dt, use_periodic, trace_file );

    } 
    catch ( boost::program_options::error& po_error ) {
        std::cerr << po_error.what() << std::endl; 
    }
    catch ( std::exception& ex ) {
        std::cerr << "Unknown exception: " << ex.what() << std::endl;
    }
}

void moldinam_basic( std::string input_file_path, std::string output_file_path, size_t iterations, double dt, bool use_periodic, std::string trace_file ) {
    std::vector<Molecule> molecules = read_molecules_from_file( input_file_path );

    std::ofstream trace;
    if ( trace_file != "" ) {
        write_molecules_to_file( molecules, trace_file );
        trace.open( trace_file.c_str(), std::ofstream::out | std::ofstream::app );
        trace << std::endl;
    }

    euler_step( molecules, dt );

    double3 area_size = { 10, 10, 10 };
    for ( size_t i = 0; i < iterations; i++ ) {
        verlet_step( molecules, dt );

        if ( use_periodic ) {
            periodic( molecules, area_size );
        }

        if ( trace_file != "" ) {
            for ( auto& mol : molecules ) {
                trace << std::endl << mol;
            }
            trace << std::endl;
        }
    }

    write_molecules_to_file( molecules, output_file_path );
}
