#include <GL/glext.h>
#include <GL/gl.h>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class particle_renderer
{
public:
    particle_renderer();
    ~particle_renderer();

    void display();
    void set_positions( std::vector< glm::vec3 >& );

protected: 
    void _initGL();
    void _drawPoints();
    GLuint _compileProgram(const char *vsource, const char *fsource);

protected:
    std::vector< glm::vec3 > positions;

    float pointSize;
    float particleRadius;
    int window_w, window_h;

    GLuint program;

    GLuint VBO;
};
