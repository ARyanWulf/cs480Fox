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
    void setCubeStatusT(char set);
    void setCubeStatusR(char set);

    int getPlanCursor();
    int getNumPlan();
    void incCursor();
    void decCursor();
    void resetCursor(int end);
    void zoomIn(int factor);
    void zoomOut(int factor);
    void resetZoom(glm::vec3 newV);

  private:
    std::string ErrorString(GLenum error);

    Camera *m_camera;
    Shader *m_shader;

    GLint m_projectionMatrix;
    GLint m_viewMatrix;
    GLint m_modelMatrix;

    std::vector<Object> m_planets;
    Object *moon_cube;
    Object *m_sun;

    int numPlan;
    int planetCursor;
};

#endif /* GRAPHICS_H */
