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

    Camera *m_camera;
    Shader *m_shader;
    Physics *world;

    GLint m_projectionMatrix;
    GLint m_viewMatrix;
    GLint m_modelMatrix;

    vector<Object*> objects;
};

#define BIT(x) (1 << (x))
enum collisiontypes{
 COL_NOTHING = 0,
 COL_BOARD = BIT(0),
 COL_BALL = BIT(1),
 COL_CYL = BIT(2),
 COL_PAD = BIT(3)
};

#endif /* GRAPHICS_H */
