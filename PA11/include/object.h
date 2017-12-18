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
    void Update(unsigned int dt, int index, Physics*);
    void Update(glm::mat4 parent, float angle);
    //void Update(unsigned int dt, Object body);
    bool loadOBJ(string path);
    void Render();
    bool initScene(const aiScene* scene, string path);
    bool initMesh(unsigned int index, const aiMesh* paiMesh);
    bool initMaterials(const aiScene* scene, const string& Filename);
    bool toOriginalPostion();

    friend class Graphics;
    glm::mat4 GetModel();
    void translateModel(int objectIndex,btVector3 newM, Physics* world, float charAngle);
    void translateModel(int objectIndex,int, Physics* world, float charAngle);
    void rotateModel(int objectIndex,btQuaternion newM, Physics* world, float);
    //float getAngle();

  private:
    bool moved;
    glm::mat4 model;
    glm::mat4 movedModel;
    glm::mat4 originModel;
    std::vector<Vertex> Normals;
    std::vector<Vertex> Vertices;
    std::vector<unsigned int> Indices;
    GLuint VB;
    GLuint IB;
    GLuint aTexture;
    Magick::Image* m_pImage;
    Magick::Blob m_blob;
    btTransform* originalTrans = NULL;

    btTriangleMesh *objTriMesh;
    vector<btRigidBody> bodies;
    btCollisionShape* shape;
};

#endif /* OBJECT_H */
