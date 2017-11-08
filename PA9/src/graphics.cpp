#include "graphics.h"

Graphics::Graphics()
{

}

Graphics::~Graphics()
{

}

bool Graphics::Initialize(int width, int height)
{

  // init spot
  intensify = 4.0;
  spotAperture = 30;

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


  //gContactAddedCallback = world->callBack;

  Object *ball = new Object("obj/ball.obj");
  Object *board = new Object("obj/board.obj");
  Object *cyl = new Object("obj/cyl.obj");
  Object *paddle = new Object("obj/cube.obj");
  objects.push_back(ball);
  objects.push_back(cyl);
  objects.push_back(paddle);
  objects.push_back(board);

  //Ball
  world->addObject(1,  0, 0, 5, 0, 1, 1, objects[0]->objTriMesh, btVector3(0, 0, 0), false);
  //Cyl
  world->addObject(0,  1, 0, 0, 2, 1, 1, objects[1]->objTriMesh, btVector3(0, 0, 0), false);
  //Paddle
  world->addObject(1,  2, 0, 1, -1, 1, 1, objects[2]->objTriMesh, btVector3(0, 0, 0), false);
  //Board
  world->addObject(0,  3, 0, 0, 0, 1, 1, objects[3]->objTriMesh, btVector3(0, 0, 0), false);

  shineAmb = 0.6;
  shineSpec = 1.5;

  setShader('v');
  // Set up the shaders
  m_shaderVert = new Shader();
  m_shaderVert->setShader('v');
cout << "shader set" << endl;
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

  // Locate the Shininess in the shader
  m_shineV = m_shaderVert->GetUniformLocation("ShininessSpec");
  if (m_shineV == INVALID_UNIFORM_LOCATION)
  {
    printf("m_shine not found\n");
    return false;
  }

  // Locate the Shininess in the shader
  m_shineAmbV = m_shaderVert->GetUniformLocation("ShininessAmbient");
  if (m_shineAmbV == INVALID_UNIFORM_LOCATION)
  {
    printf("m_shineAmb not found\n");
    return false;
  }

  // Locate the lightPosition in the shader
  m_lightPositionV = m_shaderVert->GetUniformLocation("LightPosition");
  if (m_lightPositionV == INVALID_UNIFORM_LOCATION)
  {
    printf("m_lightPosition not found\n");
    return false;
  }

  m_spotlightPositionV = m_shaderVert->GetUniformLocation("SpotlightPosition");
  if (m_spotlightPositionV == INVALID_UNIFORM_LOCATION)
  {
    printf("m_spotlightPosition not found\n");
    return false;
  }

  m_AmbientV =  m_shaderVert->GetUniformLocation("AmbientProduct");
  if (m_AmbientV == INVALID_UNIFORM_LOCATION)
  {
    printf("m_Ambient not found\n");
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
  m_shineF = m_shaderFrag->GetUniformLocation("ShininessSpec");
  if (m_shineF == INVALID_UNIFORM_LOCATION)
  {
    printf("m_shine not found\n");
    return false;
  }

  // Locate the Shininess in the shader
  m_shineAmbF = m_shaderFrag->GetUniformLocation("ShininessAmbient");
  if (m_shineAmbF == INVALID_UNIFORM_LOCATION)
  {
    printf("m_shineAmb not found\n");
    return false;
  }

  // Locate the lightPosition in the shader
  m_lightPositionF = m_shaderFrag->GetUniformLocation("LightPosition");
  if (m_lightPositionF == INVALID_UNIFORM_LOCATION)
  {
    printf("m_lightPosition not found\n");
    return false;
  }

  // Locate the spotlightPosition in the shader
  m_spotlightPositionF = m_shaderFrag->GetUniformLocation("SpotlightPosition");
  if (m_spotlightPositionF == INVALID_UNIFORM_LOCATION)
  {
    printf("m_lightPosition not found\n");
    return false;
  }

  m_AmbientF =  m_shaderFrag->GetUniformLocation("AmbientProduct");
  if (m_AmbientV == INVALID_UNIFORM_LOCATION)
  {
    printf("m_Ambient not found\n");
    return false;
  }

  m_DiffuseF =  m_shaderFrag->GetUniformLocation("DiffuseProduct");
  if (m_DiffuseV == INVALID_UNIFORM_LOCATION)
  {
    printf("m_Diffuse not found\n");
    return false;
  }

  m_SpecularF =  m_shaderFrag->GetUniformLocation("SpecularProduct");
  if (m_SpecularV == INVALID_UNIFORM_LOCATION)
  {
    printf("m_Specular not found\n");
    return false;
  }


//cout << "SETUP" << endl;

  //enable depth testing
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  return true;
}

void Graphics::Update(unsigned int dt)
{
  // Update the object


  world->Update(dt);

  //for(int i = 0; i < objects.size(); i++)
  //{
    objects[0]->Update(dt, 0, world);
    objects[1]->Update(dt, 1, world);
  //}
}

void Graphics::Render()
{
  glm::mat4 mBall = objects[0]->GetModel();
  GLfloat spot_direction[] = {mBall[3].x, mBall[3].y, mBall[3].z};

  //clear the screen
  glClearColor(0.0, 0.0, 0.2, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


  m_AmbientV = *(glm::value_ptr(glm::vec4(1.0, 1.0, 1.0, 1.0)));
  m_DiffuseV = *(glm::value_ptr(glm::vec4(1.0, 1.0, 1.0, 1.0)));
  m_SpecularV = *(glm::value_ptr(glm::vec4(1.0, 1.0, 1.0, 1.0)));
  m_spotlightPositionV = *(glm::value_ptr(glm::vec4(1.0, 20.0, 1.0, 1.0)));

  m_AmbientF = *(glm::value_ptr(glm::vec4(1.0, 1.0, 1.0, 1.0)));
  m_DiffuseF = *(glm::value_ptr(glm::vec4(1.0, 1.0, 1.0, 1.0)));
  m_SpecularF = *(glm::value_ptr(glm::vec4(1.0, 1.0, 1.0, 1.0)));
  cout << shadeLighting << endl;

  if(shadeLighting == 'v')
  {
    // Start the correct program
    m_shaderVert->Enable();

    // Send in the projection and view to the shader
    glUniformMatrix4fv(m_projectionMatrixV, 1, GL_FALSE, glm::value_ptr(m_camera->GetProjection()));
    glUniformMatrix4fv(m_viewMatrixV, 1, GL_FALSE, glm::value_ptr(m_camera->GetView()));

    // Render the object
    for(int i = 0 ; i < objects.size() ; i++)
    {
       glUniformMatrix4fv(m_modelMatrixV, 1, GL_FALSE, glm::value_ptr(objects[i]->GetModel()));
       objects[i]->Render();
    }

    glUniform1f(m_lightPositionV, m_lightPositionV);
    glUniform1f(m_shineV, shineSpec);
    glUniform1f(m_shineAmbV, shineAmb);

    glUniform3fv(m_spotlightPositionV, 1, glm::value_ptr(m_camera->GetView()));
    glLightf(m_spotlightPositionV, GL_SPOT_CUTOFF, spotAperture);
    glLightfv(m_spotlightPositionV, GL_SPOT_DIRECTION, spot_direction);
    glLightf(m_spotlightPositionV, GL_SPOT_EXPONENT, 1);

  glEnable(m_spotlightPositionV);


    glUniform1f(m_AmbientV, m_AmbientV);
    glUniform1f(m_DiffuseV, m_DiffuseV);
    glUniform1f(m_SpecularV, m_SpecularV);
  }

  else if(shadeLighting == 'f')
  {
    // Start the correct program
    m_shaderFrag->Enable();

    // Send in the projection and view to the shader
    glUniformMatrix4fv(m_projectionMatrixF, 1, GL_FALSE, glm::value_ptr(m_camera->GetProjection()));
    glUniformMatrix4fv(m_viewMatrixF, 1, GL_FALSE, glm::value_ptr(m_camera->GetView()));

    // Render the object
    for(int i = 0 ; i < objects.size() ; i++)
    {
       glUniformMatrix4fv(m_modelMatrixF, 1, GL_FALSE, glm::value_ptr(objects[i]->GetModel()));
       objects[i]->Render();
    }

    glUniform3fv(m_lightPositionF, 1, glm::value_ptr(m_camera->GetView()));
    glUniform1f(m_shineF, shineSpec);
    glUniform1f(m_shineAmbF, shineAmb);
    glUniform1f(m_AmbientF, m_AmbientF);
    glUniform1f(m_DiffuseF, m_DiffuseF);
    glUniform1f(m_SpecularF, m_SpecularF);

    glUniform3fv(m_spotlightPositionV, 1, glm::value_ptr(m_camera->GetView()));
    glLightf(m_spotlightPositionV, GL_SPOT_CUTOFF, spotAperture);
    glLightfv(m_spotlightPositionV, GL_SPOT_DIRECTION, spot_direction);
    glLightf(m_spotlightPositionV, GL_SPOT_EXPONENT, 1);

  glEnable(m_spotlightPositionV);

  }
  // Get any errors from OpenGL
  auto error = glGetError();
  if ( error != GL_NO_ERROR )
  {
    string val = ErrorString( error );
    std::cout<< "Error initializing OpenGL! " << error << ", " << val << std::endl;
  }
}

void Graphics::setShader(char shader){
   shadeLighting = shader;
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
