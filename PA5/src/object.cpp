#include "object.h"
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
    angle = 0.0f;

    glGenBuffers(1, &VB);
    glBindBuffer(GL_ARRAY_BUFFER, VB);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * Vertices.size(), &Vertices[0], GL_STATIC_DRAW);

    glGenBuffers(1, &IB);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * Indices.size(), &Indices[0], GL_STATIC_DRAW);

    //model = glm::rotate(glm::mat4(1.0f), 90, glm::vec3(1.0, 0.0, 0.0));
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
  //float *normalArray;
  //float *uvArray;
  //int numVerts;
  //Vertex v_temp;

  Assimp::Importer importer;

  const aiScene *scene = importer.ReadFile( path.c_str(), aiProcess_Triangulate );

  if(scene)
  {
	return initScene(scene, path);
  }
  else
  {
	cout << "Failed to Load Object" << endl;
	return false;
  }
  

  
  
}

bool Object::initScene( const aiScene* scene, string path )
{
	
	const aiMesh* paiMesh = scene->mMeshes[0];
	initMesh( 0, paiMesh );

	return true;

}

bool Object::initMesh(unsigned int index, const aiMesh* paiMesh)
{
	int x = 0, y = 1, z = 0;
	const aiVector3D Zero3D( 0.0f, 0.0f, 0.0f);

	for(unsigned int i = 0 ; i < paiMesh->mNumVertices ; i++ )
	{
		if(x == 0) x = 1;
		else x = 0;
		if(y == 0) y = 1;
		else y = 0;
		if(z == 0) z = 1;
		else z = 0;
		const aiVector3D* pPos = &(paiMesh->mVertices[i]);
		//const aiVector3D* pNormal = &(paiMesh->mNormals[i]) : &Zero3D;
		
		Vertex v = {{pPos->x, pPos->y, pPos->z}, {x, y, z}};

		Vertices.push_back(v);
	}
	for(unsigned int i = 0 ; i < paiMesh->mNumFaces ; i++ )
	{
		const aiFace& Face = paiMesh->mFaces[i];
		assert(Face.mNumIndices == 3);
		Indices.push_back(Face.mIndices[0]);
		Indices.push_back(Face.mIndices[1]);
		Indices.push_back(Face.mIndices[2]);		
	}
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
  glEnableVertexAttribArray(2);

  glBindBuffer(GL_ARRAY_BUFFER, VB);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,color));

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);

  glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);

  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);

}












