#include <GL/glew.h>
#include "glfw_wrapper/glfw_wrapper.hpp"
#include "glm/glm.hpp"
#include <glm/gtc/type_ptr.hpp>

class cube_window : public glfw::window {

    public:
    cube_window() : glfw::window() {
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
        "  gl_FragColor = vec4(1.0, 1.0, 0.0, 1.0);\n"
        "}\n";

        
        GLuint vShader, fShader;

        vShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vShader, 1, &vsSource, NULL);
        glCompileShader(vShader);


        fShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fShader, 1, &fsSource, NULL);
        glCompileShader(fShader);
        GLuint program = glCreateProgram();
        glAttachShader(program, vShader);
        glAttachShader(program, fShader);
        glLinkProgram(program);

        unif_mvp = glGetUniformLocation( program, "mvp" );
        attrib_vertex = glGetAttribLocation( program, "coord" );
    }

    void init_VBO() {
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        
        std::vector< glm::vec3 > cube;
		cube.push_back( glm::vec3( -0.5, -0.5, -0.5 ) );
        cube.push_back( glm::vec3( 0.5, -0.5, -0.5 ) );
        cube.push_back( glm::vec3( 0.5, -0.5, 0.5 ) );
        cube.push_back( glm::vec3( -0.5, -0.5, 0.5 ) );
        cube.push_back( glm::vec3( 0.5, 0.5, -0.5 ) );
        cube.push_back( glm::vec3( 0.5, 0.5, 0.5 ) );
        cube.push_back( glm::vec3( -0.5, 0.5, 0.5 ) );
        cube.push_back( glm::vec3( -0.5, 0.5, -0.5 ) ); 

        glBufferData(GL_ARRAY_BUFFER, cube.size() * sizeof( glm::vec3 ), glm::value_ptr( cube ), GL_STATIC_DRAW);
    }



    virtual void draw() {
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(program); 
        glm::mat4 mvp( 1.0f );
        glUniformMatrix4fv( unif_mvp, 1, false, glm::value_ptr( mvp ) );
        glEnableVertexAttribArray(attrib_vertex);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glVertexAttribPointer(attrib_vertex, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDrawArrays( GL_LINE_LOOP, 0, 8 );
    }

    GLuint program;
    GLuint attrib_vertex;
    GLuint unif_mvp;

    GLuint VBO;
};
int main( int argc, char** argv ) {
    cube_window window;
    window.start();
}
