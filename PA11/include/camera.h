#ifndef CAMERA_H
#define CAMERA_H

#include "graphics_headers.h"

class Camera
{
  public:
    Camera();
    ~Camera();
    bool Initialize(int w, int h);
    glm::mat4 UpdateCamera(glm::mat4 model, glm::mat4 directionModel);
    //void Update(glm::mat4 model, char type);
    glm::mat4 GetProjection();
    glm::mat4 GetView();

  private:
    glm::vec3 cPostition;
    glm::vec3 cDirection;
    glm::vec3 cUp;
    glm::mat4 projection;
    glm::mat4 view;
};

#endif /* CAMERA_H */
