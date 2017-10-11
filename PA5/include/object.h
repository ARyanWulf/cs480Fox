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
#include <Magick++.h>

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
    bool loadOBJ(string objPath, string mtlPath);
    void Render();
    bool initScene(const aiScene* scene, string path);
    bool initMesh(unsigned int index, const aiMesh* paiMesh);
    bool initMaterials(const aiScene* scene, string path);

    glm::mat4 GetModel();
    //float getAngle();

  private:
    glm::mat4 model;
    std::vector<Vertex> Normals;
    std::vector<Vertex> Vertices;
    std::vector<unsigned int> Indices;
    //std::vector m_Textures;
    GLuint VB;
    GLuint IB;

    float angle, angleR;
    unsigned char rotate, translate;

};

#endif /* OBJECT_H */
