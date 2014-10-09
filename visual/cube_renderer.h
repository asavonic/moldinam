#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <string>
#include <iostream>
#include <stdexcept>

class cube_renderer {
    public:
        cube_renderer();
        cube_renderer( cube_renderer& ) = delete;

        ~cube_renderer();

        void display();

        void set_mvp( glm::mat4& _mvp );
        
    protected:
        void setup_VBO();
        void setup_program();

        GLuint create_shader( GLuint shader_type, const GLchar* source );

        GLuint VBO;
        GLint VBO_size;

        GLuint program;
        GLuint attrib_vertex;
        GLuint unif_mvp;

        static const GLchar* vsSource;
        static const GLchar* fsSource;
};
