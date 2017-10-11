#include "object.h"
#include <Magick++.h>
#include <string>
#include <fstream>

using namespace std;

Object::Object()
{
  string fileName;  


  cout << endl << "Please enter object file name: ";
  cin >> fileName;
  cout << endl;

Assimp::Importer importer;
  std::string m_fileName;
  GLuint m_textureObj;
  Magick::Image* m_pImage;
  Magick::Blob m_blob;
	aiString Path;
	cout << "Path: " << Path.C_Str();
	// create and initalalize a scene that contains all of the file data
  	const aiScene *myScene = importer.ReadFile(fileName.c_str() ,aiProcess_Triangulate);
	cout << "Path: " << Path.C_Str();
	// create a pointer to the first mesh (only one)
	aiMesh *ai_mesh = myScene->mMeshes[0];  
	cout << "Path: " << Path.C_Str();
	// initialize magick
        Magick::InitializeMagick(NULL);
	cout << "Path: " << Path.C_Str();
	// create a pointer to the first material
	const aiMaterial* pMaterial = myScene->mMaterials[1];
		cout << "Path: " << Path.C_Str();
	// get texture (initialize Path)
	pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &Path, NULL, NULL, NULL, NULL, NULL);


	// load texture 
  try {
	  m_pImage = new Magick::Image(Path.C_Str());
    m_pImage->write(&m_blob, "RGBA");
	}
  catch (Magick::Error& Error) {
    std::cout << "Error loading texture '" << fileName << "': " << Error.what() << std::endl;
  }

	// get vertices if number of vertices > 0
	if (ai_mesh->mNumVertices > 0)
	{
		// loop through vertices
	  for (unsigned int i = 0; i < ai_mesh->mNumVertices; i++)
	  {
	  	// create a 3D vector to hold vertices at ith position
		  aiVector3D ai = ai_mesh->mVertices[i];
		  // create a vec3 to hold the coordiates stored in ai
		  glm::vec3 vec = glm::vec3(ai.x, ai.y, ai.z);
			// create a 3D vector to hold texture coords
		  aiVector3D texture = ai_mesh->mTextureCoords[0][i];
			//create a vec2 to hold the texture coordiates stored in texture
		  glm::vec2 tex = glm::vec2(texture.x, texture.y);

		  // initialize a temporary Vertex with vertex coordinates and color
		  Vertex *tempVertex = new Vertex(vec, tex);
		  // push back tempVertex onto Vertices
		  Vertices.push_back(*tempVertex);
		}


		// get mesh indexes
		// loop through faces
		for (unsigned int j = 0; j < ai_mesh->mNumFaces; j++){
		  // create a face to store the jth face
		  aiFace* face = &ai_mesh->mFaces[j];
		  // if objects are not triangulated
		  if (face->mNumIndices != 3)
		  {   
		    // let user know        
		    std::cout << "Object not trianuglated\n";
		    continue;
		  }
		  // push back face indices onto Indices
		  Indices.push_back(face->mIndices[0]);
		  Indices.push_back(face->mIndices[1]);
		  Indices.push_back(face->mIndices[2]);
		}
		
	}
		// bind vertex buffers
		glGenBuffers(1, &VB);
		glBindBuffer(GL_ARRAY_BUFFER, VB);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * Vertices.size(), &Vertices[0], GL_STATIC_DRAW);

		// bind indice buffer
		glGenBuffers(1, &IB);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * Indices.size(), &Indices[0], GL_STATIC_DRAW);

		// bind texture buffer		
    glGenTextures(1, &m_textureObj);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_textureObj);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_pImage->columns(), m_pImage->rows(), -0.5, GL_RGBA, GL_UNSIGNED_BYTE, m_blob.data());
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_textureObj);

		angle = 0.0f;
}


Object::~Object()
{
  Vertices.clear();
  Indices.clear();
}

bool Object::loadOBJ(string objPath, string mtlPath)
{
  //float *vertexArray;
  //float *normalArray;
  //float *uvArray;
  //int numVerts;
  //Vertex v_temp;

  Assimp::Importer importer;

  const aiScene *scene = importer.ReadFile( objPath.c_str(), aiProcess_Triangulate );

  if(scene)
  {
	return initScene(scene, mtlPath);
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

bool Object::initMaterials(const aiScene* scene, string path)
{
	
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
		const aiVector3D* pTexCoord = paiMesh->HasTextureCoords(0) ? &(paiMesh->mTextureCoords[0][i]): &Zero3D;
		//const aiVector3D* pNormal = &(paiMesh->mNormals[i]) : &Zero3D;
		
		Vertex v = {{pPos->x, pPos->y, pPos->z}, {pTexCoord->x, pTexCoord->y}};

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












