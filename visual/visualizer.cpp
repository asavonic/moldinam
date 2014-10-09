#include <GL/glew.h>
#include "glfw_wrapper/glfw_wrapper.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#include "cube_renderer.h"
#include "particles_renderer.h"

class cube_window : public glfw::window {

    using parent_t = glfw::window;

    public:
    cube_window() : glfw::window() {
        std::vector< glm::vec3 > particles;
        particles.push_back( glm::vec3( 0.0, 0.0, 0.0 ) );
        particle_render.set_positions( particles );
    }

    virtual void draw() {
        glClearColor(0, 0, 0, 0);
        glClear(GL_COLOR_BUFFER_BIT);

        /* here goes an ugly hack:
         *      we`re using angle_y for X rotation
         *      and angle_x for Y rotation
         *
         *      TODO
         *      this must be fixed
         */
        glm::vec3 axis_x(1, 0, 0);
        glm::mat4 anim = glm::rotate(glm::mat4(1.0f), angle.y, axis_x);

        glm::vec3 axis_y(0, 1, 0);
        anim = glm::rotate( anim, angle.x, axis_y);

        glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0, -4.0));
        glm::mat4 view = glm::lookAt(glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, 0.0, -4.0), glm::vec3(0.0, 1.0, 0.0));
        glm::mat4 projection = glm::perspective(45.0f, 1.0f*width()/height(), 0.1f, 10.0f);
        glm::mat4 mvp = projection * view * model * anim;

        cube_render.set_mvp( mvp );
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  
        particle_render.display();
        cube_render.display();
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
        control();
    }

    virtual void control() {
        if ( mouse_action == GLFW_PRESS && mouse_button == GLFW_MOUSE_BUTTON_LEFT ) {
            angle.x += mouse_move.x / 10 ;
            angle.y += mouse_move.y / 10 ;
        }
    }

    glm::vec2 mouse_move;
    int mouse_action;
    int mouse_button;

    glm::vec2 angle;

    

    particle_renderer particle_render;
    cube_renderer cube_render;
};

int main( int argc, char** argv ) {
    cube_window window;
    window.start();
}
