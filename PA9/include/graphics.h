#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <iostream>
using namespace std;

#include "graphics_headers.h"
#include "camera.h"
#include "shader.h"
#include "object.h"

class Graphics
{
  public:
    Graphics();
    ~Graphics();
    bool Initialize(int width, int height);
    void Update(unsigned int dt);
    void Render();
    void setShader(char shader);
    friend class Engine;

  private:
    std::string ErrorString(GLenum error);

    Camera *m_camera;
    Shader *m_shaderVert;
    Shader *m_shaderFrag;
    Physics *world;

    GLint m_projectionMatrixV;
    GLint m_viewMatrixV;
    GLint m_modelMatrixV;
    GLint m_lightPositionV;
    GLfloat m_spotlightPositionV;
    GLint m_shineV;
    GLint m_shineAmbV;
    GLfloat m_AmbientV;
    GLfloat m_DiffuseV;
    GLfloat m_SpecularV;
    GLfloat intensify;
    int spotAperture;

    GLint m_projectionMatrixF;
    GLint m_viewMatrixF;
    GLint m_modelMatrixF;
    GLint m_lightPositionF;
    GLint m_spotlightPositionF;
    GLint m_shineF;
    GLint m_shineAmbF;
    GLfloat m_AmbientF;
    GLfloat m_DiffuseF;
    GLfloat m_SpecularF;

    vector<Object*> objects;
    float shineAmb;
    float shineSpec;
    char shadeLighting;
};

#endif /* GRAPHICS_H */
