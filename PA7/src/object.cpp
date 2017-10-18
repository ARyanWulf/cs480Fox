#include "object.h"
#include <string>
#include <fstream>

using namespace std;

GLint location;

Object::Object()
{
}

Object::Object(string fileName)
{
  Magick::InitializeMagick(NULL);


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

    glScalef(-1000,-1000,-1000);
  }
  else return;
}

Object::Object(const Object& in)
{
   name = in.name;
   orbits = in.orbits;
   radius = in.radius;
   speed = in.speed;
   rotation = in.rotation;
   model = in.model;
   Vertices = in.Vertices;
   Indices = in.Indices;
   VB = in.VB;
   IB = in.IB;
   aTexture = in.aTexture;
   m_pImage = in.m_pImage;
   m_blob = in.m_blob;
   angle = in.angle;
   angleRotate = in.angleRotate;
   keypressT = in.keypressT;
   keypressR = in.keypressR;
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


void Object::setKeyT(char toSet)
{
  keypressT = toSet;
}

void Object::setKeyR(char toSet)
{
  keypressR = toSet;
}

void Object::Update(unsigned int dt)
{ 
  cout << "update object" << endl;
  switch(keypressR){
    	case 'r': //Rotate
      		if( keypressT == 'b' ) //Would otherwise cancel out r
      		{
      		  angleRotate += dt * M_PI/5000;
      		}
      		angleRotate += dt * M_PI/5000;
     		 break;
    	case 'n': //Rotate negativ
      		if( keypressT == 'f' ) //Would otherwise cancel out n
      		{
       	 		angleRotate -= dt * M_PI/5000;
      		}
      		angleRotate -= dt * M_PI/5000;
      		break;
  }
  model = glm::scale(glm::mat4(1.0f), glm::vec3(1, 10, 1));
  model = glm::rotate(glm::mat4(1.0f), (angleRotate), glm::vec3(0.0f, 1.0f, 0.0f));
}
 

void Object::Update(unsigned int dt, Object orbit)
{ 
  cout << name << "update" << endl;
  if(orbit.name == "Sun")
  {
  	//Move in Circle
  	switch(keypressT)
  	{
   		case 'f': //Forward
     			 angle += dt * M_PI/10000;
     			 break;
   		case 'b': //Backward
      			angle -= dt * M_PI/10000;
      			break;
    		case 's': //Stop
      		//Do not update
      		break;
  	}

  //Rotate
  	switch(keypressR)
        {
    		case 'r': //Rotate
      			if( keypressT == 'b' ) //Would otherwise cancel out r
      			{
      			  angleRotate += dt * M_PI/750;
      			}
      			angleRotate += dt * M_PI/750;
     			 break;
    		case 'n': //Rotate negativ
      			if( keypressT == 'f' ) //Would otherwise cancel out n
      			{
       	 			angleRotate -= dt * M_PI/750;
      			}
      			angleRotate -= dt * M_PI/750;
      			break;
    		case 'p': //Pause
     	 		//Do not update
     	 		break;
  	}
  }
  else
  {
  	angle += dt * M_PI/10000;
 	angleRotate += dt * M_PI/750;
  }

  cout << radius << endl;
  glm::mat4 m = orbit.GetModel();

  
  cout << "r: " << radius << "angle: " << 100*sin(angle) << endl;
  glm::vec3 v = (glm::vec3( 200*radius*sin(angle*speed), 0.0, 200*radius * cos(angle*speed))) + glm::vec3(m[3]);
 
  model = glm::translate(glm::mat4(1.0f), v);
  model = glm::rotate(model, (angleRotate * (float)rotation), glm::vec3(0.0, 1.0, 0.0));
}


glm::mat4 Object::GetModel()
{
  return model;
}

void Object::Render()
{
   glActiveTexture(GL_TEXTURE0);
   glBindTexture(GL_TEXTURE_2D, aTexture);
   glEnableVertexAttribArray(location);

  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);

  glBindBuffer(GL_ARRAY_BUFFER, VB);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,uv));

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);

  glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);

  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
}





















