#include "object.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/color4.h>
#include <string>
#include <fstream>

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
  string temp;
  int i_temp = 0, inside = 0;
  bool norm = false;
  float x = 1, y = 0, z = 0;
  Vertex v_temp;
  Vertex vn_temp;

  ifstream fin;
  fin.open(path);

  if(!(fin.good()))
  {
    cout << "Object file failed to open." << endl;
    return false;
  }

  while(fin.good())
  {
    if(x == 0) x = 1;
    else if(x == 1) x = 0;
    if(y == 0) y = 1;
    else if(y == 1) y = 0;
    if(z == 0) z = 1;
    else if(z == 1) z = 0;
    fin >> temp;

    if(temp == "v")
    {
	fin >> v_temp.vertex.x;
	fin >> v_temp.vertex.y;
	fin >> v_temp.vertex.z;
	
	v_temp.color.x = x;
	v_temp.color.y = y;
	v_temp.color.z = z;

	Vertices.push_back(v_temp);
	
    }
    else if(temp == "vn")
    {
	norm = true;
	fin >> vn_temp.vertex.x;
	fin >> vn_temp.vertex.y;
	fin >> vn_temp.vertex.z;

	vn_temp.color.x = x;
	vn_temp.color.y = y;
	vn_temp.color.z = z;

	Normals.push_back(vn_temp);
    }
    else if(temp == "f" && norm)
    {
      for(int i = 0; i < 3; i++)
      {
	i_temp = 0;
	inside = 0;
        while(temp[inside] != '/')
	{
	   i_temp = (i_temp * 10) + (temp[inside] - '0');
           inside++;
	}
	if(i_temp > 0) Indices.push_back(i_temp);

      }
    }
    else if(temp == "f" && !norm)
    {
      for(int i = 0; i < 3; i++)
      {
	fin >> temp;
	Indices.push_back(stoi(temp, nullptr));
      }
    }
  }
  fin.close();
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
