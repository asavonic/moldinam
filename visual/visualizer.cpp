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

class SimpleVisualizer : public glfw::window 
{
    using glfw::window::window;

    virtual void draw() override {
        float ratio = 0;
        ratio = width()/ ( float ) height();
        glViewport ( 0, 0, width(), height() );
        glClear ( GL_COLOR_BUFFER_BIT );
        glMatrixMode ( GL_PROJECTION );
        glLoadIdentity();
        glOrtho ( -ratio, ratio, -1.f, 1.f, 1.f, -1.f );
        glMatrixMode ( GL_MODELVIEW );
        glLoadIdentity();
        glRotatef ( ( float ) glfwGetTime() * 50.f, 0.f, 0.f, 1.f );
        glBegin ( GL_TRIANGLES );
        glColor3f ( 1.f, 0.f, 0.f );
        glVertex3f ( -0.6f, -0.4f, 0.f );
        glColor3f ( 0.f, 1.f, 0.f );
        glVertex3f ( 0.6f, -0.4f, 0.f );
        glColor3f ( 0.f, 0.f, 1.f );
        glVertex3f ( 0.f, 0.6f, 0.f );
        glEnd();
    }
};

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

        SimpleVisualizer visualizer( 1024, 768, "Moldynam test visualizer" );
        visualizer.start();            

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

