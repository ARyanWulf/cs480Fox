#include "graphics.h"
#include <fstream>

Graphics::Graphics()
{

}

Graphics::~Graphics()
{

}

bool Graphics::Initialize(int width, int height)
{

  // init spot

  spotAperture = 30;
  charAngle = 0.0;
  moveAngle = 0.0;

  // Used for the linux OS
  #if !defined(__APPLE__) && !defined(MACOSX)
    // cout << glewGetString(GLEW_VERSION) << endl;
    glewExperimental = GL_TRUE;

    auto status = glewInit();

    // This is here to grab the error that comes from glew init.
    // This error is an GL_INVALID_ENUM that has no effects on the performance
    glGetError();

    //Check for error
    if (status != GLEW_OK)
    {
      std::cerr << "GLEW Error: " << glewGetErrorString(status) << "\n";
      return false;
    }
  #endif

  // For OpenGL 3
  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  // Init Camera
  m_camera = new Camera();
  if(!m_camera->Initialize(width, height))
  {
    printf("Camera Failed to Initialize\n");
    return false;
  }

  // Init Physics
  world = new Physics();
  if(!world->Initialize())
  {
    printf("Physics Failed to Initialize\n");
    return false;
  }


  btCollisionShape *tempShape;
  btTransform trans;
  btScalar m[16];
  //gContactAddedCallback = world->callBack;

  Object *labyrinth = new Object("obj/laby.obj");
  Object *cam = new Object("obj/cube.obj");
  Object *direct = new Object("obj/cube.obj");
  Object *barl = new Object("obj/barl.obj");
  Object *blue = new Object("obj/blue.obj");
  Object *red = new Object("obj/redBall.obj");
  Object *yellow = new Object("obj/yellowball.obj");
  objects.push_back(labyrinth);
  objects.push_back(cam);
  objects.push_back(direct);
  objects.push_back(barl);
  objects.push_back(blue);
  objects.push_back(red);
  objects.push_back(yellow);

//MAZE ///////////////////////////////////////////////////////////////////////////////////////////////////
  world->addObject(0,  0, 0, 0, 0, 1, 1, objects[0]->objTriMesh, btVector3(0, 0, 0), true);

  world->addObject(1,  1, 0, 0, 0, 1, 1, objects[1]->objTriMesh, btVector3(0, 0, 0), false);
  world->addObject(0,  2, 0, 0, 0, 1, 1, objects[2]->objTriMesh, btVector3(0, 0, 0), true);
world->addObject(1,  3, 0, 0, 0, 1, 1, objects[3]->objTriMesh, btVector3(0, 0, 0), false);
world->addObject(1,  4, 0, 0, 0, 1, 1, objects[4]->objTriMesh, btVector3(0, 0, 0), false);
world->addObject(1,  5, 0, 0, 0, 1, 1, objects[5]->objTriMesh, btVector3(0, 0, 0), false);
world->addObject(1,  6, 0, 0, 0, 1, 1, objects[6]->objTriMesh, btVector3(0, 0, 0), false);

  trans.setOrigin(btVector3(0, 150, 0));
  objects[0]->model = glm::translate(objects[0]->model, glm::vec3(0, 150, 0));
  trans.setRotation(btQuaternion(0,0,0,1));
  world->bodies[0]->setWorldTransform(trans);
  world->bodies[0]->getMotionState()->setWorldTransform(trans);

  tempShape = world->bodies[0]->getCollisionShape();
  //tempShape->setLocalScaling(btVector3(20, 20, 20));
  //objects[0]->model = ;
  world->bodies[0]->setCollisionShape(  tempShape  );
  trans.getOpenGLMatrix(m);
  objects[0]->model = glm::make_mat4(m);
  //objects[0]->model = glm::scale(objects[0]->model, glm::vec3(20,20,20));


  //objects[0]->model = glm::scale( glm::vec3(20, 20, 20));
  objects[0]->model = glm::rotate( objects[0]->model, 45.55f, glm::vec3(0.0f , 1.0f, 0.0f));

//CAMERA CUBES ///////////////////////////////////////////////////////////////////////////////////////////
  trans.setOrigin(btVector3(0, 170, 0));
  objects[1]->model = glm::translate(objects[1]->model, glm::vec3(0, 170, 0));
  trans.setRotation(btQuaternion(0,0,0,1));
  world->bodies[1]->setWorldTransform(trans);
  world->bodies[1]->getMotionState()->setWorldTransform(trans);

  tempShape = world->bodies[1]->getCollisionShape();
  world->bodies[1]->setCollisionShape(  tempShape  );
  trans.getOpenGLMatrix(m);
  objects[1]->model = glm::make_mat4(m);
//////////////////////////////////////////////////////////////////////////
  trans.setOrigin(btVector3(20, 170, 0));
  objects[2]->model = glm::translate(objects[2]->model, glm::vec3(2, 170, 0));
  trans.setRotation(btQuaternion(0,0,0,1));
  world->bodies[2]->setWorldTransform(trans);
  world->bodies[2]->getMotionState()->setWorldTransform(trans);

  tempShape = world->bodies[2]->getCollisionShape();
  world->bodies[2]->setCollisionShape(  tempShape  );
  trans.getOpenGLMatrix(m);
  objects[2]->model = glm::make_mat4(m);
//FUN ROOM /////////////////////////////////////////////////////////////////////////////
  trans.setOrigin(btVector3(-68, 170, 9));
  objects[3]->model = glm::translate(objects[3]->model, glm::vec3(-64, 160, 9));
  trans.setRotation(btQuaternion(0,0,0,1));
  world->bodies[3]->setWorldTransform(trans);
  world->bodies[3]->getMotionState()->setWorldTransform(trans);

  tempShape = world->bodies[3]->getCollisionShape();
  world->bodies[3]->setCollisionShape(  tempShape  );
  trans.getOpenGLMatrix(m);
  objects[3]->model = glm::make_mat4(m);


  trans.setOrigin(btVector3(-68, 180, 11));
  objects[4]->model = glm::translate(objects[4]->model, glm::vec3(-66, 160, 11));
  trans.setRotation(btQuaternion(0,0,0,1));
  world->bodies[4]->setWorldTransform(trans);
  world->bodies[4]->getMotionState()->setWorldTransform(trans);

  tempShape = world->bodies[4]->getCollisionShape();
  world->bodies[4]->setCollisionShape(  tempShape  );
  trans.getOpenGLMatrix(m);
  objects[4]->model = glm::make_mat4(m);

  trans.setOrigin(btVector3(-68, 160, 13));
  objects[5]->model = glm::translate(objects[5]->model, glm::vec3(-68, 160, 13));
  trans.setRotation(btQuaternion(0,0,0,1));
  world->bodies[5]->setWorldTransform(trans);
  world->bodies[5]->getMotionState()->setWorldTransform(trans);

  tempShape = world->bodies[5]->getCollisionShape();
  world->bodies[5]->setCollisionShape(  tempShape  );
  trans.getOpenGLMatrix(m);
  objects[5]->model = glm::make_mat4(m);

  trans.setOrigin(btVector3(-68, 190, 15));
  objects[6]->model = glm::translate(objects[6]->model, glm::vec3(-70, 160, 15));
  trans.setRotation(btQuaternion(0,0,0,1));
  world->bodies[6]->setWorldTransform(trans);
  world->bodies[6]->getMotionState()->setWorldTransform(trans);

  tempShape = world->bodies[6]->getCollisionShape();
  world->bodies[6]->setCollisionShape(  tempShape  );
  trans.getOpenGLMatrix(m);
  objects[6]->model = glm::make_mat4(m);





////////
  ambControl = .5;
  diffControl = 0.5;
  specControl = 0.5;
  shineDiffuse = 0.5;
  shininessControl = 0.5;

  setShader('v');
  // Set up the shaders
  m_shaderVert = new Shader();
  m_shaderVert->setShader('v');
  if(!m_shaderVert->Initialize())
  {
    printf("Shader Failed to Initialize\n");
    return false;
  }

  // Add the vertex shader
  if(!m_shaderVert->AddShader(GL_VERTEX_SHADER))
  {
    printf("Vertex Shader failed to Initialize\n");
    return false;
  }

  // Add the fragment shader
  if(!m_shaderVert->AddShader(GL_FRAGMENT_SHADER))
  {
    printf("Fragment Shader failed to Initialize\n");
    return false;
  }

  // Connect the program
  if(!m_shaderVert->Finalize())
  {
    printf("Program to Finalize\n");
    return false;
  }

  // Locate the projection matrix in the shader
  m_projectionMatrixV = m_shaderVert->GetUniformLocation("projectionMatrix");
  if (m_projectionMatrixV == INVALID_UNIFORM_LOCATION)
  {
    printf("m_projectionMatrix not found\n");
    return false;
  }

  // Locate the view matrix in the shader
  m_viewMatrixV = m_shaderVert->GetUniformLocation("viewMatrix");
  if (m_viewMatrixV == INVALID_UNIFORM_LOCATION)
  {
    printf("m_viewMatrix not found\n");
    return false;
  }

  // Locate the model matrix in the shader
  m_modelMatrixV = m_shaderVert->GetUniformLocation("modelMatrix");
  if (m_modelMatrixV == INVALID_UNIFORM_LOCATION)
  {
    printf("m_modelMatrix not found\n");
    return false;
  }

  // Locate the lightPosition in the shader
  m_lightPositionV = m_shaderVert->GetUniformLocation("LightPosition");
  if (m_lightPositionV == INVALID_UNIFORM_LOCATION)
  {
    printf("m_lightPosition not found\n");
    return false;
  }

  m_AmbientV =  m_shaderVert->GetUniformLocation("AmbientProduct");
  if (m_AmbientV == INVALID_UNIFORM_LOCATION)
  {
    printf("m_Ambient not found\n");
    return false;
  }

  m_AmbControlV =  m_shaderVert->GetUniformLocation("ShininessAmbient");
  if (m_AmbientV == INVALID_UNIFORM_LOCATION)
  {
    printf("m_AmbControl not found\n");
    return false;
  }

  m_DiffuseV =  m_shaderVert->GetUniformLocation("DiffuseProduct");
  if (m_DiffuseV == INVALID_UNIFORM_LOCATION)
  {
    printf("m_Diffuse not found\n");
    return false;
  }

  m_SpecularV =  m_shaderVert->GetUniformLocation("SpecularProduct");
  if (m_SpecularV == INVALID_UNIFORM_LOCATION)
  {
    printf("m_Specular not found\n");
    return false;
  }

  // Locate the Shininess in the shader
  m_shininessV = m_shaderVert->GetUniformLocation("ShininessSpec");
  if (m_shininessV == INVALID_UNIFORM_LOCATION)
  {
    printf("m_shineShiniess not found\n");
    return false;
  }


  m_slAmbientProductV = m_shaderVert->GetUniformLocation("SLAmbientProduct");
  if (m_slAmbientProductV == INVALID_UNIFORM_LOCATION)
  {
    printf("m_slAmbientProductV not found\n");
    return false;
  }

  m_slDiffuseProductV = m_shaderVert->GetUniformLocation("SLDiffuseProduct");
  if (m_slDiffuseProductV == INVALID_UNIFORM_LOCATION)
  {
    printf("m_slDiffuseProductV not found\n");
    return false;
  }

  m_slSpecularProductV = m_shaderVert->GetUniformLocation("SLSpecularProduct");
  if (m_slSpecularProductV == INVALID_UNIFORM_LOCATION)
  {
    printf("m_spotlightPositionV not found\n");
    return false;
  }

  setShader('f');
  // Set up the shaders
  m_shaderFrag = new Shader();
  m_shaderFrag->setShader('f');
  if(!m_shaderFrag->Initialize())
  {
    printf("Shader Failed to Initialize\n");
    return false;
  }

  // Add the vertex shader
  if(!m_shaderFrag->AddShader(GL_VERTEX_SHADER))
  {
    printf("Vertex Shader failed to Initialize\n");
    return false;
  }

  // Add the fragment shader
  if(!m_shaderFrag->AddShader(GL_FRAGMENT_SHADER))
  {
    printf("Fragment Shader failed to Initialize\n");
    return false;
  }

  // Connect the program
  if(!m_shaderFrag->Finalize())
  {
    printf("Program to Finalize\n");
    return false;
  }

  // Locate the projection matrix in the shader
  m_projectionMatrixF = m_shaderFrag->GetUniformLocation("projectionMatrix");
  if (m_projectionMatrixF == INVALID_UNIFORM_LOCATION)
  {
    printf("m_projectionMatrix not found\n");
    return false;
  }

  // Locate the view matrix in the shader
  m_viewMatrixF = m_shaderFrag->GetUniformLocation("viewMatrix");
  if (m_viewMatrixF == INVALID_UNIFORM_LOCATION)
  {
    printf("m_viewMatrix not found\n");
    return false;
  }

  // Locate the model matrix in the shader
  m_modelMatrixF = m_shaderFrag->GetUniformLocation("modelMatrix");
  if (m_modelMatrixF == INVALID_UNIFORM_LOCATION)
  {
    printf("m_modelMatrix not found\n");
    return false;
  }

  // Locate the Shininess in the shader
  m_lightPositionF = m_shaderFrag->GetUniformLocation("LightPosition");
  if (m_lightPositionF == INVALID_UNIFORM_LOCATION)
  {
    printf("m_lightPositionF not found\n");
    return false;
  }

  // Locate the Shininess in the shader
  m_AmbientF = m_shaderFrag->GetUniformLocation("AmbientProduct");
  if (m_AmbientF == INVALID_UNIFORM_LOCATION)
  {
    printf("m_AmbientF not found\n");
    return false;
  }


  m_AmbControlF = m_shaderFrag->GetUniformLocation("ShininessAmbient");
  if (m_AmbControlF == INVALID_UNIFORM_LOCATION)
  {
    printf("m_AmbientF not found\n");
    return false;
  }

  // Locate the lightPosition in the shader
  m_DiffuseF = m_shaderFrag->GetUniformLocation("DiffuseProduct");
  if (m_DiffuseF == INVALID_UNIFORM_LOCATION)
  {
    printf("m_DiffuseF not found\n");
    return false;
  }


  m_shininessF =  m_shaderFrag->GetUniformLocation("ShininessSpecular");
  if (m_shininessF == INVALID_UNIFORM_LOCATION)
  {
    printf("m_shininessF not found\n");
    return false;
  }

  m_spotlightPositionF =  m_shaderFrag->GetUniformLocation("SpotlightPosition");
  if (m_spotlightPositionF == INVALID_UNIFORM_LOCATION)
  {
    printf("m_spotlightPositionF not found\n");
    return false;
  }

  m_slAmbientProductF =  m_shaderFrag->GetUniformLocation("SLAmbientProduct");
  if (m_slAmbientProductF == INVALID_UNIFORM_LOCATION)
  {
    printf("m_slAmbientProductF not found\n");
    return false;
  }

  m_slDiffuseProductF =  m_shaderFrag->GetUniformLocation("SLDiffuseProduct");
  if (m_slDiffuseProductF == INVALID_UNIFORM_LOCATION)
  {
    printf("m_slDiffuseProductF not found\n");
    return false;
  }

  m_slSpecularProductF =  m_shaderFrag->GetUniformLocation("SLSpecularProduct");
  if (m_slSpecularProductF == INVALID_UNIFORM_LOCATION)
  {
    printf("m_slSpecularProductF not found\n");
    return false;
  }
  //enable depth testing
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  return true;
}

void Graphics::moveChar(glm::vec3 translate, float angle){
//translateModel(int objectIndex,btQuaternion newM, Physics* world
	//objects[1]->translateModel(1, btVector3(translate.x, translate.y, translate.z), world);
	//objects[1]->model = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(1,0,0));
}

glm::mat4 Graphics::getCharMVP(){
	return objects[0]->GetModel();
}

void Graphics::Update(unsigned int dt)
{
  // Update the object
  ofstream fout;
  fout.open("log.txt");

  world->Update(dt);
  fout << objects[1]->model[3].x <<  " " << objects[1]->model[3].y <<  " "  << objects[1]->model[3].z <<  " "  << endl; 
  //for(int i = 0; i < objects.size(); i++)
  //{
    objects[0]->Update(dt, 0, world);
    objects[1]->Update(dt, 1, world);
    objects[2]->Update(objects[1]->model, charAngle);
    objects[3]->Update(dt, 3, world);
    objects[4]->Update(dt, 4, world);
    objects[5]->Update(dt, 5, world);
    objects[6]->Update(dt, 6, world);
    //m_camera->UpdateCamera(objects[1]->GetModel(), charAngle);
  //}

  fout.close();
}

void Graphics::Render()
{


  //clear the screen
  glClearColor(0.0, 0.0, 0.2, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



  if(shadeLighting == 'v')
  {
    // Start the correct program
    m_shaderVert->Enable();

    // Send in the projection and view to the shader
    glUniformMatrix4fv(m_projectionMatrixV, 1, GL_FALSE, glm::value_ptr(m_camera->GetProjection()));
    glUniformMatrix4fv(m_viewMatrixV, 1, GL_FALSE, glm::value_ptr(m_camera->GetView()));

    // Render the object
    for(int i = 0 ; i < 1 ; i++)
    {
       glUniformMatrix4fv(m_modelMatrixV, 1, GL_FALSE, glm::value_ptr(objects[i]->GetModel()));
       objects[i]->Render();
    }

    for(int i = 3 ; i < objects.size() ; i++)
    {
       glUniformMatrix4fv(m_modelMatrixV, 1, GL_FALSE, glm::value_ptr(objects[i]->GetModel()));
       objects[i]->Render();
    }

    glUniform4f(m_lightPositionV, (GLfloat)5.0,(GLfloat)5.0,(GLfloat)0.0,(GLfloat)1.0);
    glUniform1f(m_shininessV, shininessControl);
    glUniform1f(m_AmbControlV, ambControl);
    glUniform4f(m_AmbientV, ambControl, ambControl, ambControl, 1.0);
    glUniform4f(m_DiffuseV, diffControl, diffControl, diffControl, 1.0);
    glUniform4f(m_SpecularV, specControl, specControl, specControl, 1.0);

    glEnable(m_spotlightPositionV);
  }

  else if(shadeLighting == 'f')
  {
    // Start the correct program
    m_shaderFrag->Enable();

    // Send in the projection and view to the shader
    glUniformMatrix4fv(m_projectionMatrixF, 1, GL_FALSE, glm::value_ptr(m_camera->GetProjection()));
    glUniformMatrix4fv(m_viewMatrixF, 1, GL_FALSE, glm::value_ptr(m_camera->GetView()));

    // Render the object
    for(int i = 0 ; i < 1 ; i++)
    {
       glUniformMatrix4fv(m_modelMatrixF, 1, GL_FALSE, glm::value_ptr(objects[i]->GetModel()));
       objects[i]->Render();
    }

    for(int i = 3 ; i < objects.size() ; i++)
    {
       glUniformMatrix4fv(m_modelMatrixF, 1, GL_FALSE, glm::value_ptr(objects[i]->GetModel()));
       objects[i]->Render();
    }


    glUniform4f(m_lightPositionF, 0.0,20.0,0.0,1.0);
    glUniform1f(m_shininessF, shininessControl);
    glUniform1f(m_AmbControlF, ambControl);
    glUniform4f(m_AmbientF, ambControl, ambControl, ambControl, 1.0);
    glUniform4f(m_DiffuseF, diffControl, diffControl, diffControl, 1.0);
    glUniform4f(m_SpecularF, specControl, specControl, specControl, 1.0);
    glEnable(m_spotlightPositionF);

  }
  // Get any errors from OpenGL
  auto error = glGetError();
  if ( error != GL_NO_ERROR )
  {
    string val = ErrorString( error );
    std::cout<< "Error initializing OpenGL! " << error << ", " << val << std::endl;
  }
}

void Graphics::setShader(char shader)
{
   shadeLighting = shader;
}

Camera*Graphics::getCamera()
{
  return m_camera;
}

std::string Graphics::ErrorString(GLenum error)
{
  if(error == GL_INVALID_ENUM)
  {
    return "GL_INVALID_ENUM: An unacceptable value is specified for an enumerated argument.";
  }

  else if(error == GL_INVALID_VALUE)
  {
    return "GL_INVALID_VALUE: A numeric argument is out of range.";
  }

  else if(error == GL_INVALID_OPERATION)
  {
    return "GL_INVALID_OPERATION: The specified operation is not allowed in the current state.";
  }

  else if(error == GL_INVALID_FRAMEBUFFER_OPERATION)
  {
    return "GL_INVALID_FRAMEBUFFER_OPERATION: The framebuffer object is not complete.";
  }

  else if(error == GL_OUT_OF_MEMORY)
  {
    return "GL_OUT_OF_MEMORY: There is not enough memory left to execute the command.";
  }
  else
  {
    return "None";
  }
}
