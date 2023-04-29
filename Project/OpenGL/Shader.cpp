#include "Shader.h"
Shader::Shader(GLenum type, const std::string& filename) :
    m_type(type),
    m_filename(filename),
    m_id(0),
    m_compiled(false)
{   
    std::string path = "Resources/Shaders/";

    if (type==GL_VERTEX_SHADER)
        path += "Vertex/";
    else
        path += "Fragment/";

    path += filename;
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << path << std::endl;
        return;
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string source = buffer.str();
    const GLchar* source_ptr = source.c_str();
    m_id = glCreateShader(type);
    glShaderSource(m_id, 1, &source_ptr, NULL);
}

Shader::~Shader()
{
    glDeleteShader(m_id);
}

bool Shader::compile()
{
    glCompileShader(m_id);
    GLint success;
    glGetShaderiv(m_id, GL_COMPILE_STATUS, &success);
    if (success == GL_TRUE) {
        m_compiled = true;
    }
    else {
        GLchar log[1024];
        GLsizei length;
        glGetShaderInfoLog(m_id, 1024, &length, log);
        std::cerr << "Shader compilation error (" << m_filename << "): " << log << std::endl;
    }
    return m_compiled;
}

GLuint Shader::getID() const
{
    return m_id;
}