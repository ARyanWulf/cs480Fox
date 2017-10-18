#include "graphics.h"
#include<fstream>

Graphics::Graphics()
{

}

Graphics::~Graphics()
{

}

void Graphics::setCubeStatusT( char set )
{
	for(int i = 0 ; i < m_planets.size() ; i++)
	{
  		if(m_planets[i].orbits == "Sun")
		{
  			m_planets[i].setKeyT(set);
		}
	}
}

void Graphics::setCubeStatusR( char set )
{

	for(int i = 0 ; i < m_planets.size() ; i++)
	{
		if(m_planets[i].orbits == "Sun" || m_planets[i].name == "Sun" )
		{
  			m_planets[i].setKeyR(set);
		}
	}
}

bool Graphics::Initialize(int width, int height)
{
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
  // Variable declaration
  string fileName, hold, holdHead, holdData;
  string name, model, orbits;
  double temp, tempDecimal, radius, speed, rotation;
  int i, dec;
  ifstream config;
  Object *m_model;

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

// Read Config file
  cout << "Please enter config file name: ";
  cin >> fileName;

  config.open(fileName.c_str());
  cout << "good" << endl;

  if(config.good())
  {
    getline(config, hold);
    if(hold == "Start Config File")
    {
      while(hold != "End Config File")
      {
        getline(config, hold);
        cout << hold << endl;

        // gets data header
        holdHead = hold.substr(0, hold.find(":") + 1);

        // gets data
        holdData = hold.substr(hold.find(":") + 2, hold.size() - 1);
        cout << holdHead << " " << holdData << endl;
        if(holdHead == "Body:")
        {
          name = holdData;
        }
	else if(holdHead == "Model:")
	{
          model = holdData;
          m_model = new Object(model);
          m_model->name = name;
	}
	else if(holdHead == "Orbits:")
	{
         m_model->orbits = holdData;
	}
	else if(holdHead == "Orbit Radius:")
	{
	  i = 0;
	  dec = 0;
          temp = 0.0;
          tempDecimal = 0.0;
	  while(holdData[i] != '.' && i < holdData.size())
	  {
	    temp = (temp * 10) + (holdData[i] - '0');
	    i++;
	  }
	  if(holdData[i] == '.')
	  {
	    i++;
	    while(i < holdData.size())
	    {
	      tempDecimal = (tempDecimal * 10) + (holdData[i] - '0');
	      dec++;
	      i++;
	    }
	    for(int k = 0; k < dec; k++)
	    {
	      tempDecimal /= 10;
	    }
	    m_model->radius = temp + tempDecimal;
	  }
	  else 
	  {
	    m_model->radius = temp;
	  }
	}
	else if(holdHead == "Orbit Speed:")
	{
	  i = 0;
	  dec = 0;
          temp = 0.0;
          tempDecimal = 0.0;
	  while(holdData[i] != '.' && i < holdData.size())
	  {
	    temp = (temp * 10) + (holdData[i] - '0');
	    i++;
	  }
	  if(holdData[i] == '.')
	  {
	    i++;
	    while(i < holdData.size())
	    {
	      tempDecimal = (tempDecimal * 10) + (holdData[i] - '0');
	      dec++;
	      i++;
	    }
	    for(int k = 0; k < dec; k++)
	    {
	      tempDecimal /= 10;
	    }
	    m_model->speed = temp + tempDecimal;
	  }
	  else 
	  {
	    m_model->speed = temp;
	  }
	}
	else if(holdHead == "Rotation:")
	{
	  i = 0;
	  dec = 0;
          temp = 0.0;
          tempDecimal = 0.0;
	  while(holdData[i] != '.' && i < holdData.size())
	  {
	    temp = (temp * 10) + (holdData[i] - '0');
	    i++;
	  }
	  if(holdData[i] == '.')
	  {
	    i++;
	    while(i < holdData.size())
	    {
	      tempDecimal = (tempDecimal * 10) + (holdData[i] - '0');
	      dec++;
	      i++;
	    }
	    for(int k = 0; k < dec; k++)
	    {
	      tempDecimal /= 10;
	    }
	    m_model->radius = temp + tempDecimal;
	  }
	  else m_model->radius = temp;
        }
	else if(hold == "**LOAD**")
	{
          cout << m_model->name << endl;
		cout << m_model->orbits << endl;
	  m_planets.push_back(*m_model);
          delete m_model;
	}
      }
    }
  }

  // Create the object
  numPlan = 9;
  planetCursor = numPlan;

  // Set up the shaders
  m_shader = new Shader();
  if(!m_shader->Initialize())
  {
    printf("Shader Failed to Initialize\n");
    return false;
  }

  // Add the vertex shader
  if(!m_shader->AddShader(GL_VERTEX_SHADER))
  {
    printf("Vertex Shader failed to Initialize\n");
    return false;
  }

  // Add the fragment shader
  if(!m_shader->AddShader(GL_FRAGMENT_SHADER))
  {
    printf("Fragment Shader failed to Initialize\n");
    return false;
  }

  // Connect the program
  if(!m_shader->Finalize())
  {
    printf("Program to Finalize\n");
    return false;
  }

  // Locate the projection matrix in the shader
  m_projectionMatrix = m_shader->GetUniformLocation("projectionMatrix");
  if (m_projectionMatrix == INVALID_UNIFORM_LOCATION) 
  {
    printf("m_projectionMatrix not found\n");
    return false;
  }

  // Locate the view matrix in the shader
  m_viewMatrix = m_shader->GetUniformLocation("viewMatrix");
  if (m_viewMatrix == INVALID_UNIFORM_LOCATION) 
  {
    printf("m_viewMatrix not found\n");
    return false;
  }

  // Locate the model matrix in the shader
  m_modelMatrix = m_shader->GetUniformLocation("modelMatrix");
  if (m_modelMatrix == INVALID_UNIFORM_LOCATION) 
  {
    printf("m_modelMatrix not found\n");
    return false;
  }

  //enable depth testing
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  return true;
}

void Graphics::Update(unsigned int dt)
{
  std::string planNames[9] = {"Mercury", "Venus", "Earth", "Mars", "Juptier", "Saturn", "Uranus", "Neptune", "Pluto"}; planetCursor = numPlan;
  cout << "update " << m_planets.size() << endl;

  for(int i = 0; i < m_planets.size(); i++)
  {
	cout << m_planets[i].name << " orbit is " << m_planets[i].speed << endl;
    if(m_planets[i].orbits != "none") //if not sun
    {
      for(int k = 0; k < m_planets.size(); k++)
      {
	if(m_planets[i].orbits == m_planets[k].name)//checks to see which object is rotating around which object
	{
      	  m_planets[i].Update(dt, m_planets[k]);
	  break;
	}
      }
     
      if(planetCursor <= numPlan - 1)
      {
        if(planNames[planetCursor] == m_planets[i].name)
        {
           m_camera->Update(m_planets[i].GetModel(), 'p');//update camera after all objects have been updated
        }
      }
    }
    else
    {
        m_planets[i].Update(dt);//update sun
	if(planetCursor == numPlan)
          m_camera->Update(m_planets[i].GetModel(), 's');
    }
  }
}

void Graphics::Render()
{
  //clear the screen
  glClearColor(0.0, 0.0, 0.2, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Start the correct program
  m_shader->Enable();

  // Send in the projection and view to the shader
  glUniformMatrix4fv(m_projectionMatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetProjection())); 
  glUniformMatrix4fv(m_viewMatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetView())); 

  // Render the object
  for(int i = 0; i < m_planets.size(); i++)
  {
    glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_planets[i].GetModel()));
    m_planets[i].Render();
    cout << m_planets[i].name << "render" << endl;
  }

  // Get any errors from OpenGL
  auto error = glGetError();
  if ( error != GL_NO_ERROR )
  {
    string val = ErrorString( error );
    std::cout<< "Error initializing OpenGL! " << error << ", " << val << std::endl;
  }
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


int Graphics::getPlanCursor(){
	return planetCursor;
}

int Graphics::getNumPlan(){
	return numPlan;
}
void Graphics::incCursor(){
	planetCursor++;
}

void Graphics::decCursor(){
	planetCursor--;
}
void Graphics::resetCursor(int end){
	if(end == 0)
	{
		planetCursor = 0;
	}
	else
	{
		planetCursor = numPlan;
	}
}

void Graphics::zoomIn(int factor){
	m_camera->zoomIn(factor);
}

void Graphics::zoomOut(int factor){
	m_camera->zoomOut(factor);
}

void Graphics::resetZoom(glm::vec3 newV){
	m_camera->resetOff(newV);
}
