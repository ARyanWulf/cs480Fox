#ifndef CAMERA_H
#define CAMERA_H

#include "graphics_headers.h"

class Camera
{
  public:
    Camera();
    ~Camera();
    bool Initialize(int w, int h);
    glm::mat4 GetProjection();
    glm::mat4 GetView();
    void Update(glm::mat4 model, char type);

    void zoomIn(int add);
    void zoomOut(int add);
    void resetOff(glm::vec3 newV);

  
  private:
    glm::mat4 projection;
    glm::mat4 view;
    glm::vec3 cameraPosition;
    glm::vec3 offset;
};

#endif /* CAMERA_H */
