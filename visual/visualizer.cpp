#include <GL/glut.h>
#include <glm/glm.hpp>
#include <vector>
#include <md_types.h>
#include <md_helpers.h>
#include <iostream>
#include <boost/program_options.hpp>
#include <functional>
#include <memory>

namespace po = boost::program_options;

int mouse_x_old  = 0;
int mouse_y_old  = 0;
int mouse_x  = 0;
int mouse_y  = 0;
int angle_x = 0;
int angle_y = 0;
int eye_dist = 0;


void setMatrix(void) {
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-2.0, 2.0, -2.0, 2.0, -2.0, 2.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

void resize(int w, int h) {
  glViewport(0, 0, w, h);
  setMatrix();
}

void mouse_callback(int button, int state, int x, int y) {
	if (state == GLUT_UP) {
		if (button == 3)
			eye_dist += 0.2;
		else if (button == 4) {
			eye_dist -= 0.2;
        }
	}
    else {
        mouse_x = x;
        mouse_y = y;
        mouse_x_old = mouse_x;
        mouse_y_old = mouse_y;
    }
}

void mousemotion(int x, int y) {
	mouse_x_old = mouse_x;
	mouse_y_old = mouse_y;
    mouse_x = x;
    mouse_y = y;
    
    angle_x += mouse_x_old - mouse_x;
    angle_y += mouse_y_old - mouse_y;

	glutPostRedisplay();
}

class Display {
public:
    virtual void draw_scene() = 0;
    virtual void update() {};
};

class State_display : public Display {
public:
    State_display( std::string _state_file ) {
        state_file = _state_file;
        molecules = read_molecules_from_file( state_file );
    }

    virtual void draw_scene() {
        glPushMatrix();
            glTranslated(0.5, 0.5, 0.5);

            glTranslatef( -0.5, -0.5, -0.5 );
            glRotatef( angle_y, 1.0, 0.0, 0.0);
            glRotatef( angle_x, 0.0, 1.0, 0.0);

            glTranslated(-0.5, -0.5, -0.5);

            draw_cube();

        glPopMatrix();
    }

    virtual void draw_molecules() {
        glColor3f(1.0, 0.0, 0.0);
        for ( auto& mol : molecules ) {
            glTranslatef( mol.pos.x, mol.pos.y, mol.pos.z );
            glutSolidSphere( 0.02, 10, 10);
            glTranslatef( -mol.pos.x, -mol.pos.y, -mol.pos.z );
        }
    }

    virtual void draw_cube() {
        
        std::vector< glm::vec3 > cube;
		cube.push_back( glm::vec3( 0.0, 0.0, 0.0 ) );
        cube.push_back( glm::vec3( 1.0, 0.0, 0.0 ) );
        cube.push_back( glm::vec3( 1.0, 0.0, 1.0 ) );
        cube.push_back( glm::vec3( 0.0, 0.0, 1.0 ) );
        cube.push_back( glm::vec3( 1.0, 1.0, 0.0 ) );
        cube.push_back( glm::vec3( 1.0, 1.0, 1.0 ) );
        cube.push_back( glm::vec3( 0.0, 1.0, 1.0 ) );
        cube.push_back( glm::vec3( 0.0, 1.0, 0.0 ) ); 

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

        draw_molecules();

        glutSwapBuffers();
    }
protected:
    State_display() {} ;

    std::string state_file;
    std::vector<Molecule> molecules;
};

class Trace_display : public State_display {
public:
    Trace_display( std::string trace_file ) {
        trace.open( trace_file );
        molecules = trace.initial();
    }

    virtual void draw_scene() {
        State_display::draw_scene();
    }

    virtual void update() {
        if ( trace.active ) {
            molecules = trace.next();
        }
    }

protected:
    trace_read trace;
};

std::shared_ptr<Display> g_display_ptr;

void draw_scene() {
    g_display_ptr->draw_scene();
}

void idle() {
    g_display_ptr->update();
    glutPostRedisplay();
}

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

        glutInit(&argc, argv);

        glutInitWindowSize(1000, 1000);
        glutInitDisplayMode(GLUT_RGB | GLUT_STENCIL | GLUT_DOUBLE | GLUT_DEPTH);
        glutCreateWindow("Moldinam visualizer");

        if ( vm.count("state") ) {
            g_display_ptr.reset( new State_display( state_file ) );
        } 
        else {
            g_display_ptr.reset( new Trace_display( trace_file ) );
        }

        glutDisplayFunc( draw_scene );
        glutReshapeFunc(resize);
        glutMouseFunc(mouse_callback);
        glutMotionFunc(mousemotion);
        glutIdleFunc(idle);

        glutMainLoop();

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

