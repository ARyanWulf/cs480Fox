#ifndef SHADER_H
#define SHADER_H

#include <vector>

#include "graphics_headers.h"

class Shader
{
  public:
    Shader();
    ~Shader();
    bool Initialize();
    void Enable();
    bool AddShader(GLenum ShaderType);
    bool Finalize();
    GLint GetUniformLocation(const char* pUniformName);
    void setShader(char shaderFile);
    friend class Graphics;

  private:
    GLuint m_shaderProg;    
    std::vector<GLuint> m_shaderObjList;
    char shading;
};

#endif  /* SHADER_H */
