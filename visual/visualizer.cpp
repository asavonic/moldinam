#include <md_types.h>
#include <md_helpers.h>
#include <glm/glm.hpp>
#include <vector>
#include <iostream>
#include <boost/program_options.hpp>
#include <functional>
#include <memory>

#include "glfw_wrapper/glfw_wrapper.hpp"

namespace po = boost::program_options;


int main( int argc, char** argv ) {
    try {
        std::string  state_file;
        std::string  trace_file;

        // named arguments
        po::options_description desc("Allowed options");
        desc.add_options()
            ("help", "produce help message")
            ("state", po::value< std::string >( &state_file ), "view state file (input or output)")
            ("trace", po::value< std::string >( &trace_file ), "view trace file with animation")
        ;

        po::variables_map vm;
        po::store( po::command_line_parser( argc, argv).options(desc).run(), vm );

        if (vm.count("help")) {
            std::cout << desc << std::endl;
            return 1;
        }

        if ( vm.count("state") + vm.count("trace") > 1 ||
                vm.count( "state" ) + vm.count("trace") == 0 )
        {
            throw po::error( "State OR trace file must be specified" );
        }

        po::notify(vm);

        glfw::window window( 1024, 768, "Moldynam test visualizer" );

    } 
    catch ( boost::program_options::error& po_error ) {
        std::cerr << po_error.what() << std::endl; 
        return 1;
    }
    catch ( std::exception& ex ) {
        std::cerr << "Unknown exception: " << ex.what() << std::endl;
        return 1;
    }

    return 0;
}

