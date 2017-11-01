#include "object.h"
#include <string>
#include <fstream>

using namespace std;

GLint location;

Object::Object()
{
  objTriMesh = new btTriangleMesh();
  Magick::InitializeMagick(NULL);
  string fileName;
  string mtlFile;


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
   glActiveTexture(GL_TEXTURE0);
   glBindTexture(GL_TEXTURE_2D, aTexture);
  //
   glEnableVertexAttribArray(location);
    glGenTextures(1, &aTexture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, aTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_pImage->columns(), m_pImage->rows(), 0, GL_RGBA, GL_UNSIGNED_BYTE, m_blob.data());
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //model = glm::rotate(glm::mat4(1.0f), 90, glm::vec3(1.0, 0.0, 0.0));
  }
  shape = new btBvhTriangleMeshShape(objTriMesh, true)
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



  // glActiveTexture(GL_TEXTURE0);
  // glBindTexture(GL_TEXTURE_2D, aTexture);
  //
  // glEnableVertexAttribArray(location);
}

bool Object::initScene( const aiScene* scene, string path)
{
  const aiMesh* paiMesh = scene->mMeshes[0];
	initMesh( 0, paiMesh );
  initMaterials(scene, path);


	return true;
}

bool Object::initMesh(unsigned int index, const aiMesh* paiMesh)
{
        btVector3 triArray[3];
	const aiVector3D Zero3D( 0.0f, 0.0f, 0.0f);
	vector<Vertex> tempV;

	for(unsigned int i = 0 ; i < paiMesh->mNumVertices ; i++ )
	{
		const aiVector3D* pPos = &(paiMesh->mVertices[i]);
		const aiVector3D* luv = paiMesh->HasTextureCoords(0) ? &(paiMesh->mTextureCoords[0][i]): &Zero3D;

		Vertex v = {{pPos->x, pPos->y, pPos->z}, {luv->x, luv->y}};
                
		Vertices.push_back(v);
	}
	for(unsigned int i = 0 ; i < paiMesh->mNumFaces ; i++ )
	{
		const aiFace& Face = paiMesh->mFaces[i];
		assert(Face.mNumIndices == 3);
		Indices.push_back(Face.mIndices[0]);
		Indices.push_back(Face.mIndices[1]);
		Indices.push_back(Face.mIndices[2]);
		bodies.push_back(Face.mIndices[0], Face.mIndices[1], Face.mIndices[2]);
	}
	for(unsigned int i = 0; i < Vertices.size(); i += 3)
	{
		triArray[0] = btVector3(Vertices[i].vertex.x, Vertices[i].vertex.y, Vertices[i].vertex.z);
		triArray[1] = btVector3(Vertices[i+1].vertex.x, Vertices[i+1].vertex.y, Vertices[i+1].vertex.z);
		triArray[2] = btVector3(Vertices[i+2].vertex.x, Vertices[i+2].vertex.y, Vertices[i+2].vertex.z);
		objTriMesh->addTriangle(triArray[0], triArray[1], triArray[2]);
	}
	return true;
}

bool Object::initMaterials(const aiScene* scene, const string& Filename)
{
  aiString Path;


		const aiMaterial* pMaterial = scene->mMaterials[1];
		cout << "Num of Materials " << scene->mNumMaterials << endl;
		cout << "Here" << endl;
		if(pMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0)
		{
			if( (pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &Path, NULL, NULL, NULL, NULL, NULL)) == AI_SUCCESS)
			{
				cout << "Path: " << Path.C_Str() << endl;

				//ERROR PUTTING INFO IN PATH
				try{
					m_pImage = new Magick::Image(Path.C_Str());
					m_pImage->write(&m_blob, "RGBA");
				}
				catch(Magick::Error& Error){
					cout << "Error loading texture " << Path.C_Str() << ": " << Error.what() << endl;
				}
			}

		}
		else
		{
			cout << "Failure to load texture path" <<endl;
		}
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
  return angle;luv->x, luv->y
}*/

glm::mat4 Object::GetModel()
{
  return model;
}

void Object::Render()
{
   glActiveTexture(GL_TEXTURE0);
   glBindTexture(GL_TEXTURE_2D, aTexture);
  //
   glEnableVertexAttribArray(location);

  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glEnableVertexAttribArray(2);

  glBindBuffer(GL_ARRAY_BUFFER, VB);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,uv));
  //glVertexAttribPointer(location, 2, GL_FLOAT, GL_FALSE, );

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);

  glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);

  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);

}
