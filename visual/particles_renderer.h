#ifndef __PARTICLES_RENDERER_H
#define __PARTICLES_RENDERER_H

#include <GL/glext.h>
#include <GL/gl.h>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <md_types.h>

#include "renderer.hpp"

class ParticleRenderer : public ShaderRenderer
{
public:
    ParticleRenderer();
    ~ParticleRenderer();

    void display();
    void set_positions( std::vector< glm::vec3 >& );

    void set_particles_positions( std::vector<Molecule> molecules );
    glm::mat3 get_particles_scale_matrix( double3 area_size );

protected: 
    void setup_program();

    void _drawPoints();

protected:
    std::vector< glm::vec3 > positions;

    float pointSize;
    float particleRadius;
    int window_w, window_h;

    GLuint program;
    GLuint VBO;

    GLuint attrib_vertex;

    static const char *vertex_shader_source;
    static const char *fragment_shader_source;
};

#endif
