#ifndef OBJECT_H
#define OBJECT_H

#include <vector>
#include "graphics_headers.h"
#include <string>
#include <math.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/color4.h>

using namespace std;

class Object
{
  public:
    Object();
    ~Object();
    void setRotate(char in);
    void setTranslate(char in);
    void Update(unsigned int dt);
    //void Update(unsigned int dt, Object body);
    bool loadOBJ(string path);
    void Render();
    bool initScene(const aiScene* scene, string path);
    bool initMesh(unsigned int index, const aiMesh* paiMesh);

    glm::mat4 GetModel();
    //float getAngle();

  private:
    glm::mat4 model;
    std::vector<Vertex> Normals;
    std::vector<Vertex> Vertices;
    std::vector<unsigned int> Indices;
    GLuint VB;
    GLuint IB;

    float angle, angleR;
    unsigned char rotate, translate;

};

#endif /* OBJECT_H */
