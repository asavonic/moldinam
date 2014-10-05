#include <GL/glew.h>
#include "glfw_wrapper/glfw_wrapper.hpp"
#include "glm/glm.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

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

        shaderLog( vShader );

        fShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fShader, 1, &fsSource, NULL);
        glCompileShader(fShader);

        shaderLog( fShader );
        program = glCreateProgram();
        glAttachShader(program, vShader);
        glAttachShader(program, fShader);
        glLinkProgram(program);

        checkOpenGLerror();
        unif_mvp = glGetUniformLocation( program, "mvp" );
        attrib_vertex = glGetAttribLocation( program, "coord" );
    }

void shaderLog(unsigned int shader) 
{ 
  int   infologLen   = 0;
  int   charsWritten = 0;
  char *infoLog;

  glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infologLen);

  if(infologLen > 1)
  { 
    infoLog = new char[infologLen];
    if(infoLog == NULL)
    {
      std::cout<<"ERROR: Could not allocate InfoLog buffer\n";
       exit(1);
    }
    glGetShaderInfoLog(shader, infologLen, &charsWritten, infoLog);
    std::cout<< "InfoLog: " << infoLog << "\n\n\n";
    delete[] infoLog;
  }
}

void checkOpenGLerror()
{
  GLenum errCode;
  if((errCode=glGetError()) != GL_NO_ERROR)
    std::cout << "OpenGl error! - " << gluErrorString(errCode) << std::endl;
}
    void init_VBO() {
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        
		cube.push_back( glm::vec3( -1.0, -1.0, 1.0 ) );
        cube.push_back( glm::vec3( 1.0, -1.0, 1.0 ) );
        cube.push_back( glm::vec3( 1.0, -1.0, 1.0 ) );
        cube.push_back( glm::vec3( 1.0, 1.0, 1.0 ) );
        cube.push_back( glm::vec3( 1.0, 1.0, 1.0 ) );
        cube.push_back( glm::vec3( -1.0, 1.0, 1.0 ) );
        cube.push_back( glm::vec3( -1.0, 1.0, 1.0 ) );
		cube.push_back( glm::vec3( -1.0, -1.0, 1.0 ) );

		cube.push_back( glm::vec3( -1.0, -1.0, 1.0 ) );
		cube.push_back( glm::vec3( -1.0, -1.0, -1.0 ) );
		cube.push_back( glm::vec3( 1.0, -1.0, 1.0 ) );
		cube.push_back( glm::vec3( 1.0, -1.0, -1.0 ) );
		cube.push_back( glm::vec3( 1.0, 1.0, 1.0 ) );
		cube.push_back( glm::vec3( 1.0, 1.0, -1.0 ) );
		cube.push_back( glm::vec3( -1.0, 1.0, 1.0 ) );
		cube.push_back( glm::vec3( -1.0, 1.0, -1.0 ) );

		cube.push_back( glm::vec3( -1.0, -1.0, -1.0 ) );
        cube.push_back( glm::vec3( 1.0, -1.0, -1.0 ) );
        cube.push_back( glm::vec3( 1.0, -1.0, -1.0 ) );
        cube.push_back( glm::vec3( 1.0, 1.0, -1.0 ) );
        cube.push_back( glm::vec3( 1.0, 1.0, -1.0 ) );
        cube.push_back( glm::vec3( -1.0, 1.0, -1.0 ) );
        cube.push_back( glm::vec3( -1.0, 1.0, -1.0 ) );
		cube.push_back( glm::vec3( -1.0, -1.0, -1.0 ) );

        glBufferData(GL_ARRAY_BUFFER, cube.size() * sizeof( glm::vec3 ), glm::value_ptr( cube[0] ), GL_STATIC_DRAW);
    }



    virtual void draw() {
        glClearColor(0, 0, 0, 0);
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(program); 


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

        glUniformMatrix4fv( unif_mvp, 1, false, glm::value_ptr( mvp ) );
        glEnableVertexAttribArray(attrib_vertex);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glVertexAttribPointer(attrib_vertex, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDrawArrays( GL_LINES, 0, cube.size() );
        checkOpenGLerror();
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
