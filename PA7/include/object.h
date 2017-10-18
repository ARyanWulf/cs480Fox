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

class Object
{
  public:
    Object(std::string fileName);
    Object(const Object& in);
    ~Object();
    void Update(unsigned int dt);
    void Update(unsigned int dt, Object cube);
    void Render();
    void setKeyT( char toSet);
    void setKeyR( char toSet);
    friend class Graphics;

    glm::mat4 GetModel();

    bool loadOBJ(std::string path);
    bool initScene(const aiScene* scene, std::string path);
    bool initMesh(unsigned int index, const aiMesh* paiMesh);
    bool initMaterials(const aiScene* scene, const std::string& Filename);

  private:
    std::string name, orbits;
    double radius, speed, rotation;
    glm::mat4 model;
    std::vector<Vertex> Vertices;
    std::vector<unsigned int> Indices;
    GLuint VB;
    GLuint IB;
    GLuint aTexture;
    Magick::Image* m_pImage;
    Magick::Blob m_blob;

    float angle;
    float angleRotate;
    char keypressT = 'f';
    char keypressR = 'r';
};

#endif /* OBJECT_H */
