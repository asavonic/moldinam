#include <GL/glew.h>
#include <GL/glut.h>

#include "render_particles.h"
#include "shaders.h"
#include <iostream>

particle_renderer::particle_renderer()
: pointSize(1.0f),
  particleRadius(0.125f * 0.5f),
  program(0),
  VBO(0)
{
    _initGL();
}

particle_renderer::~particle_renderer()
{
}

void particle_renderer::set_positions( std::vector< glm::vec3 >& _positions )
{
    positions = _positions;
}

void particle_renderer::_drawPoints()
{
    if (!VBO)
    {
        glBegin(GL_POINTS);
        {
            for ( size_t i = 0; i < positions.size(); ++i )
            {
                glVertex3fv( glm::value_ptr( positions[i] ) );
            }
        }
        glEnd();
    }
    else
    {
        glBindBufferARB(GL_ARRAY_BUFFER_ARB, VBO);
        glVertexPointer(4, GL_FLOAT, 0, 0);
        glEnableClientState(GL_VERTEX_ARRAY);                

        glDrawArrays( GL_POINTS, 0, positions.size() );

        glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
        glDisableClientState(GL_VERTEX_ARRAY); 
        glDisableClientState(GL_COLOR_ARRAY); 
    }
}

void particle_renderer::display()
{
        glEnable(GL_POINT_SPRITE_ARB);
        glTexEnvi(GL_POINT_SPRITE_ARB, GL_COORD_REPLACE_ARB, GL_TRUE);
        glEnable(GL_VERTEX_PROGRAM_POINT_SIZE_NV);
        glDepthMask(GL_TRUE);
        glEnable(GL_DEPTH_TEST);

        glUseProgram(program);
        /* glUniform1f( glGetUniformLocation(program, "pointScale"), window_h / tan(fov*0.5*M_PI/180.0) ); */
        /* glUniform1f( glGetUniformLocation(program, "pointRadius"), particleRadius ); */
        glUniform1f( glGetUniformLocation(program, "pointScale"), 0.01f );
        glUniform1f( glGetUniformLocation(program, "pointRadius"), 0.0001 );

        glColor3f(1, 1, 1);
        _drawPoints();

        glUseProgram(0);
        glDisable(GL_POINT_SPRITE_ARB);
}

GLuint
particle_renderer::_compileProgram(const char *vsource, const char *fsource)
{
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(vertexShader, 1, &vsource, 0);
    glShaderSource(fragmentShader, 1, &fsource, 0);
    
    glCompileShader(vertexShader);
    glCompileShader(fragmentShader);


    GLuint program = glCreateProgram();

    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);

    glLinkProgram(program);

    // check if program linked
    GLint success = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &success);

    if (!success) {
        char temp[256];
        glGetProgramInfoLog(program, 256, 0, temp);
        printf("Failed to link program:\n%s\n", temp);
        glDeleteProgram(program);
        program = 0;
    }

    return program;
}

void particle_renderer::_initGL()
{
    program = _compileProgram(sphereVertexShader, spherePixelShader);

    glClampColorARB(GL_CLAMP_VERTEX_COLOR_ARB, GL_FALSE);
    glClampColorARB(GL_CLAMP_FRAGMENT_COLOR_ARB, GL_FALSE);
}
