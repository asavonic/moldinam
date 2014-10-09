#include <GL/glew.h>
#include "glfw_wrapper/glfw_wrapper.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>


#include "render_particles.h"

class cube_window : public glfw::window {

    using parent_t = glfw::window;

    public:
    cube_window() : glfw::window() {
        init_program();
        init_VBO();
    }
    void init_program() {
        const GLchar* vsSource = 
        "attribute vec3 coord;\n"
        "uniform mat4 mvp;\n"
        "void main() {\n"
        "  gl_Position = mvp * vec4(coord, 1.0);\n"
        "}\n";


        const GLchar* fsSource = 
        "void main() {\n"
        "  gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);\n"
        "}\n";

        
        GLuint vShader, fShader;

        vShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vShader, 1, &vsSource, NULL);
        glCompileShader(vShader);

        fShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fShader, 1, &fsSource, NULL);
        glCompileShader(fShader);

        program = glCreateProgram();
        glAttachShader(program, vShader);
        glAttachShader(program, fShader);
        glLinkProgram(program);

        unif_mvp = glGetUniformLocation( program, "mvp" );
        attrib_vertex = glGetAttribLocation( program, "coord" );
    }

    void init_VBO() {
		cube.push_back( glm::vec3( .0, .0, .0 ) );

        particle_render.set_positions( cube );
    }



    virtual void draw() {

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  
        particle_render.display();
    }

    virtual void mouse_move_callback( double new_x, double new_y ) {
        static double old_x = new_x;
        static double old_y = new_y;

        mouse_move.x = new_x - old_x;
        mouse_move.y = new_y - old_y;

        old_x = new_x;
        old_y = new_y;

        control();
    }

    virtual void mouse_press_callback( int button, int action, int mods ) {
        mouse_button = button;
        mouse_action = action;
        std::cout << "button" << std::endl;
        control();
    }

    virtual void control() {
        if ( mouse_action == GLFW_PRESS && mouse_button == GLFW_MOUSE_BUTTON_LEFT ) {
            angle.x += mouse_move.x / 10 ;
            angle.y += mouse_move.y / 10 ;
            std::cout << "move x = " << mouse_move.x << " y = " << mouse_move.y << std::endl;
        }
    }

    glm::vec2 mouse_move;
    int mouse_action;
    int mouse_button;

    particle_renderer particle_render;

    GLuint program;
    GLuint attrib_vertex;
    GLuint unif_mvp;
    std::vector< glm::vec3 > cube;

    GLuint VBO;
    glm::vec2 angle;
};
int main( int argc, char** argv ) {
    cube_window window;
    window.start();
}
