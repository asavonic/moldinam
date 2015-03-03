#include "cube_renderer.h"

CubeRenderer::CubeRenderer() {
    setup_VBO();
    setup_program();
}

CubeRenderer::~CubeRenderer() {
    //TODO:
    //  add resource handling
}

void CubeRenderer::display() {
    glUseProgram(program); 

    use_mvp();

    glEnableVertexAttribArray(attrib_vertex);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(attrib_vertex, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDrawArrays( GL_LINES, 0, VBO_size );

    glUseProgram( 0 );
}

#define STRINGIFY(A) #A

const GLchar* CubeRenderer::vsSource = STRINGIFY( 
    attribute vec3 coord;
    uniform mat4 mvp;
    void main() {
        gl_Position = mvp * vec4(coord, 1.0);
    } 
);

const GLchar* CubeRenderer::fsSource = STRINGIFY( 
    void main() {
        gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);
    }
);

#undef STRINGIFY

void CubeRenderer::setup_program() {
    GLuint vertex_shader = create_shader( GL_VERTEX_SHADER, vsSource );
    GLuint fragment_shader = create_shader( GL_FRAGMENT_SHADER, fsSource );

    program = glCreateProgram();
    glAttachShader( program, vertex_shader );
    glAttachShader( program, fragment_shader );
    glLinkProgram(program);

    unif_mvp = glGetUniformLocation( program, "mvp" );
    attrib_vertex = glGetAttribLocation( program, "coord" );
}

void CubeRenderer::setup_VBO() {
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    std::vector< glm::vec3 > cube = {
		glm::vec3( -1.0, -1.0, 1.0 ),
        glm::vec3( 1.0, -1.0, 1.0 ),
        glm::vec3( 1.0, -1.0, 1.0 ),
        glm::vec3( 1.0, 1.0, 1.0 ),
        glm::vec3( 1.0, 1.0, 1.0 ),
        glm::vec3( -1.0, 1.0, 1.0 ),
        glm::vec3( -1.0, 1.0, 1.0 ),
		glm::vec3( -1.0, -1.0, 1.0 ),

		glm::vec3( -1.0, -1.0, 1.0 ),
		glm::vec3( -1.0, -1.0, -1.0 ),
		glm::vec3( 1.0, -1.0, 1.0 ),
		glm::vec3( 1.0, -1.0, -1.0 ),
		glm::vec3( 1.0, 1.0, 1.0 ),
		glm::vec3( 1.0, 1.0, -1.0 ),
		glm::vec3( -1.0, 1.0, 1.0 ),
		glm::vec3( -1.0, 1.0, -1.0 ),

		glm::vec3( -1.0, -1.0, -1.0 ),
        glm::vec3( 1.0, -1.0, -1.0 ),
        glm::vec3( 1.0, -1.0, -1.0 ),
        glm::vec3( 1.0, 1.0, -1.0 ),
        glm::vec3( 1.0, 1.0, -1.0 ),
        glm::vec3( -1.0, 1.0, -1.0 ),
        glm::vec3( -1.0, 1.0, -1.0 ),
		glm::vec3( -1.0, -1.0, -1.0 )
    };

    VBO_size = cube.size();
    glBufferData(GL_ARRAY_BUFFER, VBO_size * sizeof( glm::vec3 ), glm::value_ptr( cube[0] ), GL_STATIC_DRAW);
}
