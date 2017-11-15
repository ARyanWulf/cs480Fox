#include "object.h"
#include <string>
#include <fstream>

using namespace std;

GLint location;

Object::Object(string fileName)
{
  objTriMesh = new btTriangleMesh();
  Magick::InitializeMagick(NULL);


  if(loadOBJ(fileName))
  {

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


    shape = new btBvhTriangleMeshShape(objTriMesh, true);

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
		const aiVector3D* pNormal = &(paiMesh->mNormals[i]);
		const aiVector3D* luv = paiMesh->HasTextureCoords(0) ? &(paiMesh->mTextureCoords[0][i]): &Zero3D;

		Vertex v = {{pPos->x, pPos->y, pPos->z}, 
			    {luv->x, 1 - luv->y},
		/*Normals.push_back(vec3(*/{pNormal->x, pNormal->y, pNormal->z}};
                
		Vertices.push_back(v);
	}
	for(unsigned int i = 0 ; i < paiMesh->mNumFaces ; i++ )
	{
		const aiFace& Face = paiMesh->mFaces[i];
		assert(Face.mNumIndices == 3);
		Indices.push_back(Face.mIndices[0]);
		Indices.push_back(Face.mIndices[1]);
		Indices.push_back(Face.mIndices[2]);
		//bodies.push_back(btRigidBody(Face.mIndices[0], Face.mIndices[1], Face.mIndices[2]));
	//objTriMesh->addTriangle(Face.mIndices
	}
	for(unsigned int i = 0; i < Vertices.size(); i += 3)
	{
		triArray[0] = btVector3(Vertices[i].vertex.x, Vertices[i].vertex.y, Vertices[i].vertex.z);
		triArray[1] = btVector3(Vertices[i+1].vertex.x, Vertices[i+1].vertex.y, Vertices[i+1].vertex.z);
		triArray[2] = btVector3(Vertices[i+2].vertex.x, Vertices[i+2].vertex.y, Vertices[i+2].vertex.z);
		objTriMesh->addTriangle(triArray[0], triArray[1], triArray[2]);
	}	return true;
}

bool Object::initMaterials(const aiScene* scene, const string& Filename)
{
  aiString Path;


		const aiMaterial* pMaterial = scene->mMaterials[1];
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

void Object::Update(unsigned int dt, int index, Physics* world, glm::vec3 scalar)
{
  btTransform trans; 
  btScalar m[16];
  world->bodies[index]->getMotionState()->getWorldTransform(trans);


  trans.getOpenGLMatrix(m);  
  model = glm::make_mat4(m) * glm::scale(glm::mat4(1.0f), scalar);
}

glm::mat4 Object::GetModel()
{
  return model;
}

void Object::translateModel(btQuaternion newM, Physics* world)
{
  btTransform trans;
  btTransform original;
  btVector3 oldPos;
  btQuaternion oldangle;

  world->bodies[7]->getMotionState()->getWorldTransform(original);
  oldangle = original.getRotation();
  oldPos =  original.getOrigin();
  

  
  newM = oldangle + newM;

  btScalar m[16];
  trans.setOrigin(oldPos);
  trans.setRotation(newM);


  world->bodies[7]->setWorldTransform(trans);
  world->bodies[7]->getMotionState()->setWorldTransform(trans);

  trans.getOpenGLMatrix(m);  
  model = glm::make_mat4(m);
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
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);

  glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);

  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
  glDisableVertexAttribArray(2);

}
