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
    friend class Engine;

  private:
    std::string ErrorString(GLenum error);

    camera *m_camera;
    Shader *m_shader;

    GLint m_projectionMatrix;
    GLint m_viewMatrix;
    GLint m_modelMatrix;
    Object *m_cube, *moon;

};

#endif /* GRAPHICS_H */
