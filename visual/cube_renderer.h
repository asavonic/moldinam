#ifndef __CUBE_RENDERER_H
#define __CUBE_RENDERER_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <string>
#include <iostream>
#include <stdexcept>

#include "renderer.hpp"

class cube_renderer : public shader_renderer {
    public:
        cube_renderer();
        cube_renderer( cube_renderer& ) = delete;

        ~cube_renderer();

        virtual void display();

    protected:
        void setup_VBO();
        void setup_program();

        GLuint VBO;
        GLint VBO_size;

        GLuint program;
        GLuint attrib_vertex;

        static const GLchar* vsSource;
        static const GLchar* fsSource;
};

#endif
