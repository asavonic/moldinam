/*
 * Copyright 1993-2006 NVIDIA Corporation.  All rights reserved.
 *
 * NOTICE TO USER:   
 *
 * This source code is subject to NVIDIA ownership rights under U.S. and 
 * international Copyright laws.  
 *
 * NVIDIA MAKES NO REPRESENTATION ABOUT THE SUITABILITY OF THIS SOURCE 
 * CODE FOR ANY PURPOSE.  IT IS PROVIDED "AS IS" WITHOUT EXPRESS OR 
 * IMPLIED WARRANTY OF ANY KIND.  NVIDIA DISCLAIMS ALL WARRANTIES WITH 
 * REGARD TO THIS SOURCE CODE, INCLUDING ALL IMPLIED WARRANTIES OF 
 * MERCHANTABILITY, NONINFRINGEMENT, AND FITNESS FOR A PARTICULAR PURPOSE.   
 * IN NO EVENT SHALL NVIDIA BE LIABLE FOR ANY SPECIAL, INDIRECT, INCIDENTAL, 
 * OR CONSEQUENTIAL DAMAGES, OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS 
 * OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE 
 * OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE 
 * OR PERFORMANCE OF THIS SOURCE CODE.  
 *
 * U.S. Government End Users.  This source code is a "commercial item" as 
 * that term is defined at 48 C.F.R. 2.101 (OCT 1995), consisting  of 
 * "commercial computer software" and "commercial computer software 
 * documentation" as such terms are used in 48 C.F.R. 12.212 (SEPT 1995) 
 * and is provided to the U.S. Government only as a commercial end item.  
 * Consistent with 48 C.F.R.12.212 and 48 C.F.R. 227.7202-1 through 
 * 227.7202-4 (JUNE 1995), all U.S. Government End Users acquire the 
 * source code with only those rights set forth herein.
 */

#include <GL/glew.h>
#include <GL/glut.h>

#include <math.h>
#include <assert.h>
#include <stdio.h>

#include "render_particles.h"
#include "shaders.h"
#include <iostream>

//#define GL_POINT_SPRITE_ARB               0x8861
//#define GL_COORD_REPLACE_ARB              0x8862
//#define GL_VERTEX_PROGRAM_POINT_SIZE_NV   0x8642

#ifndef M_PI
#define M_PI    3.1415926535897932384626433832795
#endif

ParticleRenderer::ParticleRenderer()
: m_pos(0),
  m_numParticles(0),
  m_pointSize(1.0f),
  m_particleRadius(0.125f * 0.5f),
  m_program(0),
  m_vbo(0),
  m_colorVBO(0)
{
    _initGL();
}

ParticleRenderer::~ParticleRenderer()
{
    m_pos = 0;
}

void ParticleRenderer::setPositions(float *pos, int numParticles)
{
    m_pos = pos;
    m_numParticles = numParticles;
}

void ParticleRenderer::setVertexBuffer(unsigned int vbo, int numParticles)
{
    m_vbo = vbo;
    m_numParticles = numParticles;
}
void ParticleRenderer::_checkShaderLog(unsigned int shader) 
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

void ParticleRenderer::_drawPoints()
{
    if (!m_vbo)
    {
        glBegin(GL_POINTS);
        {
            int k = 0;
            for (int i = 0; i < m_numParticles; ++i)
            {
                glVertex3fv(&m_pos[k]);
                k += 4;
            }
        }
        glEnd();
    }
    else
    {
        glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_vbo);
        glVertexPointer(4, GL_FLOAT, 0, 0);
        glEnableClientState(GL_VERTEX_ARRAY);                

        if (m_colorVBO) {
            glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_colorVBO);
            glColorPointer(4, GL_FLOAT, 0, 0);
            glEnableClientState(GL_COLOR_ARRAY);
        }

        glDrawArrays(GL_POINTS, 0, m_numParticles);

        glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
        glDisableClientState(GL_VERTEX_ARRAY); 
        glDisableClientState(GL_COLOR_ARRAY); 
    }
}

void ParticleRenderer::display()
{
        glEnable(GL_POINT_SPRITE_ARB);
        glTexEnvi(GL_POINT_SPRITE_ARB, GL_COORD_REPLACE_ARB, GL_TRUE);
        glEnable(GL_VERTEX_PROGRAM_POINT_SIZE_NV);
        glDepthMask(GL_TRUE);
        glEnable(GL_DEPTH_TEST);

        glUseProgram(m_program);
        /* glUniform1f( glGetUniformLocation(m_program, "pointScale"), m_window_h / tan(m_fov*0.5*M_PI/180.0) ); */
        /* glUniform1f( glGetUniformLocation(m_program, "pointRadius"), m_particleRadius ); */
        glUniform1f( glGetUniformLocation(m_program, "pointScale"), 0.01f );
        glUniform1f( glGetUniformLocation(m_program, "pointRadius"), 0.0001 );

        glColor3f(1, 1, 1);
        _drawPoints();

        glUseProgram(0);
        glDisable(GL_POINT_SPRITE_ARB);
}

GLuint
ParticleRenderer::_compileProgram(const char *vsource, const char *fsource)
{
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(vertexShader, 1, &vsource, 0);
    glShaderSource(fragmentShader, 1, &fsource, 0);
    
    glCompileShader(vertexShader);
    _checkShaderLog( vertexShader );
    glCompileShader(fragmentShader);
    _checkShaderLog( fragmentShader );


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

void ParticleRenderer::_initGL()
{
    m_program = _compileProgram(sphereVertexShader, spherePixelShader);

    glClampColorARB(GL_CLAMP_VERTEX_COLOR_ARB, GL_FALSE);
    glClampColorARB(GL_CLAMP_FRAGMENT_COLOR_ARB, GL_FALSE);
}
