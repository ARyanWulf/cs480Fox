#include "object.h"
#include <string>
#include <fstream>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/color4.h>

using namespace std;

Object::Object()
{
  string fileName;  


  cout << endl << "Please enter object file name: ";
  cin >> fileName;

  if(loadOBJ(fileName))
  {
    // The index works at a 0th index
    for(unsigned int i = 0; i < Indices.size(); i++)
    {
      Indices[i] = Indices[i] - 1;
    }

    angle = 0.0f;

    glGenBuffers(1, &VB);
    glBindBuffer(GL_ARRAY_BUFFER, VB);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * Vertices.size(), &Vertices[0], GL_STATIC_DRAW);

    glGenBuffers(1, &IB);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * Indices.size(), &Indices[0], GL_STATIC_DRAW);
  }
  else return;
}

Object::~Object()
{
  Vertices.clear();
  Indices.clear();
}

bool Object::loadOBJ(string path)
{
  //float *vertexArray;
  float *normalArray;
  float *uvArray;
  int numVerts;
  Vertex v_temp;

  Assimp::Importer importer;

  const aiScene *scene = importer.ReadFile( path, aiProcess_Triangulate );
  aiMesh *mesh = scene->mMeshes[0];

  numVerts = mesh->mNumFaces * 3;
  
  //vertexArray = new float[mesh->mNumFaces * 3 * 3];
  
  for( int i = 0 ; i < mesh->mNumFaces ; i++ )
  {
    const aiFace& face = mesh->mFaces[i];

    for( int j = 0 ; j < 3 ; j++ )
    {
      aiVector3D pos = mesh->mVertices[face.mIndices[j]];
      v_temp.vertex.x = pos.x;
      v_temp.vertex.y = pos.y;
      v_temp.vertex.z = pos.z;

      v_temp.color.x = 1;
      v_temp.color.y = 0;
      v_temp.color.z = 1;

      Vertices.push_back(v_temp);

    }

  }

  //vertexArray -= mesh->mNumFaces * 3 * 3;
  return true;
}

void Object::setRotate(char in)
{
  rotate = in;
}

void Object::setTranslate(char in)
{
  translate = in;
}

void Object::Update(unsigned int dt)
{
  glm::mat4 rotation;
  glm::mat4 translation;

  switch(translate)
  {
  case 'R':
    angle += dt *M_PI/1000;
    break;

  case 'V':
    break;

  case 'F':
    angle -= dt *M_PI/1000;
    break;

  default:
    angle += dt *M_PI/1000;
    break;
  }

  switch(rotate)
  {
  case 'T':
    angleR += dt *M_PI/800;
    break;

  case 'G':
    break;

  case 'B':
    angleR -= dt *M_PI/800;
    break;

  default:
    if (translate != 'F') angleR += dt *M_PI/800;
    else if (translate != 'R') angleR += dt *M_PI/800;
    break;
  }

  translation = glm::translate(glm::mat4(1.0f), glm::vec3(5*sin(angle), 0.0, 5*cos(angle)));
  rotation = glm::rotate(glm::mat4(1.0f), (angleR), glm::vec3(0.0, 3.0, 0.0));


  model = translation * rotation;
}
/*
void Object::Update(unsigned int dt, Object body)
{
  angle += dt *M_PI/1000;
  angleR += dt *M_PI/500;
  glm::mat4 m = body.GetModel();
  glm::vec3 v = (glm::vec3(5*sin(angle), 0.0, 5*cos(angle))) + glm::vec3(m[3]);
  glm::mat4 rotation;
  glm::mat4 translation;

  translation = glm::translate(glm::mat4(1.0f), v);
  rotation = glm::rotate(glm::mat4(1.0f), (angleR), glm::vec3(0.0, 3.0, 0.0));


  model = translation * rotation;
}

float Object::getAngle()
{
  return angle;
}*/

glm::mat4 Object::GetModel()
{
  return model;
}

void Object::Render()
{
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);

  glBindBuffer(GL_ARRAY_BUFFER, VB);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,color));

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);

  glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);

  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
}
