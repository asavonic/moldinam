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
    /* sample draw method */
    /*
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
    */
        static float angle_x = 0;
        static float angle_y = 0;
        //angle_x += 1;
        angle_y += 1;
        glPushMatrix();
            glRotatef( angle_y, 1.0, 0.0, 0.0);
            glRotatef( angle_x, 0.0, 1.0, 0.0);
            draw_cube();
        glPopMatrix();
    }

    virtual void draw_cube() 
    {
        std::vector< glm::vec3 > cube;
		cube.push_back( glm::vec3( -0.5, -0.5, -0.5 ) );
        cube.push_back( glm::vec3( 0.5, -0.5, -0.5 ) );
        cube.push_back( glm::vec3( 0.5, -0.5, 0.5 ) );
        cube.push_back( glm::vec3( -0.5, -0.5, 0.5 ) );
        cube.push_back( glm::vec3( 0.5, 0.5, -0.5 ) );
        cube.push_back( glm::vec3( 0.5, 0.5, 0.5 ) );
        cube.push_back( glm::vec3( -0.5, 0.5, 0.5 ) );
        cube.push_back( glm::vec3( -0.5, 0.5, -0.5 ) ); 

        static int faceIndex[6][4] =
        { { 0, 1, 2, 3 },
          { 1, 4, 5, 2 },
          { 4, 7, 6, 5 },
          { 7, 0, 3, 6 },
          { 3, 2, 5, 6 },
          { 7, 4, 1, 0 } };

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);

        glClearColor(0.0, 0.0, 0.0, 0.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glColor3f(1.0, 1.0, 0.0);

        for ( unsigned int face = 0; face < 6; face++ ) {
            glBegin(GL_LINE_LOOP);
                for ( unsigned int line = 0; line < 4; line++  )  {
                    glVertex3fv( (GLfloat *) &cube[faceIndex[face][line]]);
                }
            glEnd();
        }
    }

    class Rotation {
        Rotation() : active(0), _cur_pos(0), _prev_pos(0), _angle(0) {};
        void mouse_move( double new_pos_x, double new_pos_y ) {
            _prev_pos = _cur_pos;
            _cur_pos.x = new_pos_x;
            _cur_pos.y = new_pos_y;

            if ( active ) {
                _angle += _cur_pos - _prev_pos;
            }
        }

        void mouse_press_callback( int key, int scancode, int action, int mods ) {
            if ( key == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_REPEAT ) {
                active ^= 1;
            }
        }

        glm::dvec2 angle() {
            return _angle;
        }
        private:
        
        bool active;
        glm::dvec2 _cur_pos;
        glm::dvec2 _prev_pos;
        glm::dvec2 _angle;
    };
    
    public:

    /**
     * @brief Size of drawable area. All particles position will be scaled to this size.
     */
    glm::vec3 dimensions;
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

        SimpleVisualizer visualizer( 1920, 1080, "Moldynam test visualizer" );
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

