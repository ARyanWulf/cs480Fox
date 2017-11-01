#include<iostream>
#include"Physics.h"
#include <string>
#include <math.h>
#include <btBulletDynamicsCommon.h>

using namespace std;

Physics::Physics(){
   collisionConfiguration = NULL;
   dispatcher = NULL;
   broadphase = NULL;
   solver = NULL;
}

Physics::~Physics(){
   while( bodies.size() != 0 )
   {
      dynamicsWorld->removeRigidBody(bodies.back());
      delete bodies.back()->getMotionState();
      delete bodies.back();
      bodies.pop_back();
   }

   while( shapes.size() != 0 )
   {
      delete shapes.back();
      shapes.pop_back();
   }

   while( motions.size() != 0 )
   {
      delete motions.back();
      motions.pop_back();
   }

   delete broadphase;
   delete collisionConfiguration;
   delete dispatcher;
   delete solver;
   delete dynamicsWorld;

   broadphase = NULL;
   collisionConfiguration = NULL;
   dispatcher = NULL;
   solver = NULL;
   dynamicsWorld = NULL;

}

bool Physics::Initialize(){
   /*Initialization*/
   collisionConfiguration = new btDefaultCollisionConfiguration();
   dispatcher = new btCollisionDispatcher(collisionConfiguration);
   broadphase = new btDbvtBroadphase();
   solver = new btSequentialImpulseConstraintSolver;

   dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);
   if(dynamicsWorld == NULL )
   {
      return false;
   }
  /*Set Gravity*/
  dynamicsWorld->setGravity(btVector3(0,-9.81,0));
  return true;
}

/*************ADD SHAPES*************/
void Physics::addObject(int Mass, int index, int x, int y, int z, int mask, int collidesWith, btTriangleMesh* objTriMesh, btVector3 inertia, bool kinematic){

  cout << "mask: " << mask << " collidesWith: " << collidesWith << endl;

  btCollisionShape *shape = new btBvhTriangleMeshShape(objTriMesh, true);
  shapes.push_back(shape);
  btDefaultMotionState *shapeMotionState = NULL;
  shapeMotionState = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1), btVector3(x, y, z)));
  motions.push_back(shapeMotionState);

  btScalar mass(Mass);

  if(mass > 0){
     shape->calculateLocalInertia(mass, inertia);
  }

  btRigidBody::btRigidBodyConstructionInfo shapeCI(mass, shapeMotionState, shape, inertia);
  btRigidBody *rigidBody = new btRigidBody(shapeCI);
  rigidBody->setGravity(btVector3(0,-9.81,0));
  bodies.push_back(rigidBody);
  rigidBody->setActivationState(DISABLE_DEACTIVATION);
  rigidBody->setCollisionFlags(bodies[index]->getCollisionFlags() | 			btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK); // MIGHT WORK? - Blame Chris




  if(kinematic)
  {
    rigidBody->setCollisionFlags(bodies[index]->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
  }


  dynamicsWorld->addRigidBody(rigidBody, mask, collidesWith);
}


/******************************************************************* OBJECT UPDATE ****************************************************************************************/
void Physics::Update(int dt){
    dynamicsWorld->stepSimulation(dt,1);
}

bool Physics::callBack(btManifoldPoint& cp, const btCollisionObject* obj0, int id0, int index0,const btCollisionObject* obj1, int id1, int index1){

  cout << "colllision" << endl;
  return false;

}

