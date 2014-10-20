#include <GL/glew.h>
#include <GL/glut.h>

#include "particles_renderer.h"
#include <iostream>

particle_renderer::particle_renderer()
: pointSize(1.0f),
  particleRadius(0.125f * 0.5f),
  program(0),
  VBO(0)
{
    glClampColorARB(GL_CLAMP_VERTEX_COLOR_ARB, GL_FALSE);
    glClampColorARB(GL_CLAMP_FRAGMENT_COLOR_ARB, GL_FALSE);

    setup_program();
}

particle_renderer::~particle_renderer()
{
}

void particle_renderer::set_positions( std::vector< glm::vec3 >& _positions )
{
    positions = _positions;
}


void particle_renderer::set_particles_positions( std::vector<Molecule> molecules ) {
    positions.resize( molecules.size() );
    for ( size_t i = 0; i < molecules.size(); i++ ) {
        positions[i] = glm::vec3( molecules[i].pos.x, molecules[i].pos.y, molecules[i].pos.z );
    }
}
void particle_renderer::setup_program() {
    GLuint vertex_shader = create_shader( GL_VERTEX_SHADER, vertex_shader_source );
    GLuint fragment_shader = create_shader( GL_FRAGMENT_SHADER, fragment_shader_source );

    program = glCreateProgram();
    glAttachShader( program, vertex_shader );
    glAttachShader( program, fragment_shader );
    glLinkProgram(program);

    unif_mvp = glGetUniformLocation( program, "mvp" );
    attrib_vertex = glGetAttribLocation( program, "coord" );
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

#define STRINGIFY(A) #A

// vertex shader
const char * particle_renderer::vertex_shader_source = STRINGIFY(
uniform float pointRadius;  // point size in world space
uniform float pointScale;   // scale to calculate size in pixels
uniform float densityScale;
uniform float densityOffset;
uniform mat4 mvp;
void main()
{
    // calculate window-space point size
    vec3 posEye = vec3(gl_ModelViewMatrix * vec4(gl_Vertex.xyz, 1.0));
    float dist = length(posEye);
    gl_PointSize = 20.0; //pointRadius * (pointScale / dist);

    gl_TexCoord[0] = gl_MultiTexCoord0;
    gl_Position = mvp * vec4(gl_Vertex.xyz, 1.0);
    gl_FrontColor = gl_Color;
}
);

// pixel shader for rendering points as shaded spheres
const char * particle_renderer::fragment_shader_source = STRINGIFY(
vec3 lightDir = vec3(0.577, 0.577, 0.577);
void main()
{
    // calculate normal from texture coordinates
    vec3 N;
    N.xy = gl_TexCoord[0].xy*vec2(2.0, -2.0) + vec2(-1.0, 1.0);
    float mag = dot(N.xy, N.xy);
    if (mag > 1.0) discard;   // kill pixels outside circle
    N.z = sqrt(1.0-mag);

    // calculate lighting
    float diffuse = max(0.05, dot(lightDir, N));

    gl_FragColor = gl_Color * diffuse;
}
);

void particle_renderer::display()
{
        glEnable(GL_POINT_SPRITE_ARB);
        glTexEnvi(GL_POINT_SPRITE_ARB, GL_COORD_REPLACE_ARB, GL_TRUE);
        glEnable(GL_VERTEX_PROGRAM_POINT_SIZE_NV);
        glDepthMask(GL_TRUE);
        glEnable(GL_DEPTH_TEST);


        glUseProgram(program);

        use_mvp();
        /* glUniform1f( glGetUniformLocation(program, "pointScale"), window_h / tan(fov*0.5*M_PI/180.0) ); */
        /* glUniform1f( glGetUniformLocation(program, "pointRadius"), particleRadius ); */
        glUniform1f( glGetUniformLocation(program, "pointScale"), 0.01f );
        glUniform1f( glGetUniformLocation(program, "pointRadius"), 0.0001 );

        glColor3f(1, 1, 1);
        _drawPoints();

        glUseProgram(0);
        glDisable(GL_POINT_SPRITE_ARB);
}
