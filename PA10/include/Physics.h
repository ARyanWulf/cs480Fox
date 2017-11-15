#ifndef PHYSICS_H
#define PHYSICS_H

#include "graphics_headers.h"
#include <vector>
#include <iostream>
#include <string>
#include <math.h>
#include <btBulletDynamicsCommon.h>
using namespace std;

class Physics
{
  public:
    Physics();
    ~Physics();
    bool Initialize();
    void Update(int dt);
    void RenderPlane();
    void addObject(int Mass, int index, int x, int y, int z, int mask, int collidesWith, btTriangleMesh* objTriMesh, btVector3 inertia, bool kinematic);
    bool callBack(btManifoldPoint& cp, const btCollisionObject* obj0, int id0, int index0,const btCollisionObject* obj1, int id1, int index1);

    glm::mat4 GetModel();
     friend class Graphics;
     friend class Object;
     friend class Engine;
    //float getAngle();

  private:
    btBroadphaseInterface *broadphase;
    btDefaultCollisionConfiguration *collisionConfiguration;
    btCollisionDispatcher *dispatcher;
    btSequentialImpulseConstraintSolver *solver;
    btDiscreteDynamicsWorld *dynamicsWorld;

    vector<btRigidBody*> bodies;
    vector<btCollisionShape*> shapes;
    vector<btDefaultMotionState*> motions;
};

#endif
