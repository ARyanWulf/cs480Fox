#ifndef OBJECT_H
#define OBJECT_H

#include <vector>
#include "graphics_headers.h"
#include <math.h>

class Object
{
  public:
    Object();
    ~Object();
    void setRotate(char in);
    void setTranslate(char in);
    void Update(unsigned int dt);
    void Update(unsigned int dt, Object body);
    void Render();

    glm::mat4 GetModel();
    float getAngle();

  private:
    glm::mat4 model;
    std::vector<Vertex> Vertices;
    std::vector<unsigned int> Indices;
    GLuint VB;
    GLuint IB;

    float angle, angleR;
    unsigned char rotate, translate;

};

#endif /* OBJECT_H */
