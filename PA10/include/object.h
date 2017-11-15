#ifndef OBJECT_H
#define OBJECT_H

#include <vector>
#include "graphics_headers.h"
#include "Physics.h"
#include <string>
#include <math.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/color4.h>
#include <Magick++.h>
#include <btBulletDynamicsCommon.h>

using namespace std;

class Object
{
  public:
    Object(string fileName);
    ~Object();
    void setRotate(char in);
    void setTranslate(char in);
    void Update(unsigned int dt, int index, Physics*, glm::vec3);
    //void Update(unsigned int dt, Object body);
    bool loadOBJ(string path);
    void Render();
    bool initScene(const aiScene* scene, string path);
    bool initMesh(unsigned int index, const aiMesh* paiMesh);
    bool initMaterials(const aiScene* scene, const string& Filename);

    friend class Graphics;
    glm::mat4 GetModel();
    void translateModel(btQuaternion newM, Physics*);
    //float getAngle();

  private:
    glm::mat4 model;
    std::vector<Vertex> Normals;
    std::vector<Vertex> Vertices;
    std::vector<unsigned int> Indices;
    GLuint VB;
    GLuint IB;
    GLuint aTexture;
    Magick::Image* m_pImage;
    Magick::Blob m_blob;

    btTriangleMesh *objTriMesh;
    vector<btRigidBody> bodies;
    btCollisionShape* shape;
};

#endif /* OBJECT_H */
