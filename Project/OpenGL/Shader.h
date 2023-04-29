#pragma once

#include <GL/glew.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader {
public:
    Shader(GLenum type, const std::string& filename);
    ~Shader();
    bool compile();
    GLuint getID() const;
private:
    GLenum m_type;
    GLuint m_id;
    std::string m_filename;
    bool m_compiled;
};