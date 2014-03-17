#include <boost/program_options.hpp>
#include <iostream>
#include <vector> 
#include <string>

namespace po = boost::program_options;

int main( int argc, char** argv ) {
    try {
        int iterations = 0;
        double dt = 0;
        std::string input_file_path;
        std::string output_file_path;

        // named arguments
        po::options_description desc("Allowed options");
        desc.add_options()
            ("help", "produce help message")
            ("iterations", po::value<int>( &iterations )->required(), "number of iterations")
            ("dt", po::value<double>( &dt )->default_value( 0.00001 ), "dt on each step")
            ("input,i", po::value< std::string >( &input_file_path )->required(), "path to input .xyz file")
            ("output,o", po::value< std::string >( &output_file_path )->required(), "path to output .xyz file")
        ;

        // positional arguments
        po::positional_options_description p;
        p.add("input", -1);

        po::variables_map vm;
        po::store( po::command_line_parser( argc, argv).options(desc).positional(p).run(), vm );
        po::notify(vm);

        if (vm.count("help")) {
            std::cout << desc << std::endl;
                return 1;
        }

        std::cout << "input file  = " << input_file_path << std::endl;
        std::cout << "output file = " << output_file_path << std::endl;
        std::cout << "iterations  = " << iterations << std::endl;   
        std::cout << "dt          = " << dt << std::endl;   
    } 
    catch ( boost::program_options::error& po_error ) {
        std::cerr << po_error.what() << std::endl; 
    }
    catch ( std::exception& ex ) {
        std::cerr << "Unknown exception: " << ex.what() << std::endl;
    }
}
