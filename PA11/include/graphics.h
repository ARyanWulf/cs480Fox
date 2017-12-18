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
    void moveChar(glm::vec3 translate, float angle);
    glm::mat4 getCharMVP();
    Camera* getCamera();
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
    GLint m_AmbientV;
    GLint m_AmbControlV;
    GLint m_DiffuseV;
    GLint m_SpecularV;
    GLint m_shininessV;
    GLint m_spotlightPositionV;
    GLint m_slAmbientProductV;
    GLint m_slDiffuseProductV;
    GLint m_slSpecularProductV;

    GLint m_projectionMatrixF;
    GLint m_viewMatrixF;
    GLint m_modelMatrixF;
    GLint m_lightPositionF;
    GLint m_AmbientF;
    GLint m_AmbControlF;
    GLint m_DiffuseF;
    GLint m_SpecularF;
    GLint m_shininessF;
    GLint m_spotlightPositionF;
    GLint m_slAmbientProductF;
    GLint m_slDiffuseProductF;
    GLint m_slSpecularProductF;

    vector<Object*> objects;
    float ambControl;
    float diffControl;
    float specControl;
    float shineDiffuse;
    float shininessControl;
    int spotAperture;
    char shadeLighting;

    vector<btHingeConstraint*> hinge;
    float charAngle;
    float moveAngle;
};

#endif /* GRAPHICS_H */
