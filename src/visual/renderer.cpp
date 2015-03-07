#include <iostream>
#include <stdexcept>

#include <glm/gtc/type_ptr.hpp>

#include "renderer.hpp"

void ShaderRenderer::use_mvp()
{
    glUniformMatrix4fv(unif_mvp, 1, false, glm::value_ptr(mvp));
}

void ShaderRenderer::set_mvp(glm::mat4 _mvp) { mvp = _mvp; }

GLuint ShaderRenderer::create_shader(GLuint shader_type, const GLchar* source)
{
    GLuint shader = glCreateShader(shader_type);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);

    GLint log_len = 0;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_len);
    std::string log(log_len, '\0');
    glGetShaderInfoLog(shader, log.length(), NULL, &log[0]);

    GLint success = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success) {
        std::cerr << "Shader build failed -- build log:" << std::endl << log
                  << std::endl;
        throw std::runtime_error("glCompileShader failed");
    }

    return shader;
}
