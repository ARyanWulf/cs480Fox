
#include "engine.h"
#include <btBulletDynamicsCommon.h>
#include<fstream>

Engine::Engine(string name, int width, int height)
{
  m_WINDOW_NAME = name;
  m_WINDOW_WIDTH = width;
  m_WINDOW_HEIGHT = height;
  m_FULLSCREEN = false;
}

Engine::Engine(string name)
{
  m_WINDOW_NAME = name;
  m_WINDOW_HEIGHT = 0;
  m_WINDOW_WIDTH = 0;
  m_FULLSCREEN = true;
}

Engine::~Engine()
{
  delete m_window;
  delete m_graphics;
  m_window = NULL;
  m_graphics = NULL;
}

bool Engine::Initialize()
{
  // Start a window
  m_window = new Window();
  if(!m_window->Initialize(m_WINDOW_NAME, &m_WINDOW_WIDTH, &m_WINDOW_HEIGHT))
  {
    printf("The window failed to initialize.\n");
    return false;
  }

  // Start the graphics
  m_graphics = new Graphics();
  if(!m_graphics->Initialize(m_WINDOW_WIDTH, m_WINDOW_HEIGHT))
  {
    printf("The graphics failed to initialize.\n");
    return false;
  }

  // Set the time
  m_currentTimeMillis = GetCurrentTimeMillis();

  // Set Cursor to Middle of screen
  //m_graphics->glutWarpPointer(0, 0);
  return true;
}

void Engine::Run()
{
  m_running = true;

  while(m_running)
  {
    // Update the DT
    m_DT = getDT();
    // Check the keyboard input
    while(SDL_PollEvent(&m_event) != 0)
    {
      Keyboard();
      Mouse();
    }
    // Update and render the graphics
    m_graphics->Update(m_DT);
    m_graphics->Render();

    // Swap to the Window
    m_window->Swap();
  }
}

void Engine::Keyboard()
{
	float movementAng = cos(m_graphics->charAngle);
   if(m_event.type == SDL_QUIT)
   {
     m_running = false;
   }
   else if (m_event.type == SDL_KEYDOWN)
  {
    switch(m_event.key.keysym.sym)
    {
      case SDLK_a://Move Camera to the Left
	m_graphics->objects[1]->translateModel(1, btVector3(0, 0 ,.2), m_graphics->world, m_graphics->charAngle);
        m_graphics->getCamera()->UpdateCamera(m_graphics->objects[1]->GetModel(), m_graphics->objects[2]->GetModel());
        break;
      case SDLK_d://Move Camera to the Right
        m_graphics->objects[1]->translateModel(1, btVector3(0, 0 ,-0.2), m_graphics->world, m_graphics->charAngle);
        m_graphics->getCamera()->UpdateCamera(m_graphics->objects[1]->GetModel(), m_graphics->objects[2]->GetModel());
        break;
      case SDLK_w://Move Camera Forward
	if(m_graphics->moveAngle == 0)
	{
		m_graphics->objects[1]->translateModel(1, btVector3(0, 0 ,0.2), m_graphics->world, m_graphics->charAngle);
	}
	//DONe//////////////////////////////////////////////////////////////////////
	else if(3.14*6.5 > m_graphics->moveAngle && (3.14)*4 < m_graphics->moveAngle)
	{
		if(m_graphics->charAngle > 3.14)
		{
        		m_graphics->objects[1]->translateModel(1, btVector3(-.2*cos(270 - m_graphics->charAngle), 0 ,-0.2*sin(270 - m_graphics->charAngle)), m_graphics->world, m_graphics->charAngle);
		}
		else{
			m_graphics->objects[1]->translateModel(1, btVector3(.2*cos(270 - m_graphics->charAngle), 0 ,0.2*sin(270 - m_graphics->charAngle)), m_graphics->world, m_graphics->charAngle);
		}
	}
	else if(m_graphics->moveAngle == 3.14*2)
	{
		m_graphics->objects[1]->translateModel(1, btVector3(-0.2, 0 ,0), m_graphics->world, m_graphics->charAngle);
	}
	//DONe////////////////////////////////////////////////////////////////////
	else if(m_graphics->moveAngle < 3.14*2 && m_graphics->moveAngle > 0)
	{
		if(m_graphics->charAngle > 3.14)
		{
			m_graphics->objects[1]->translateModel(1, btVector3(.2*cos(180 - m_graphics->charAngle), 0 ,0.2*sin(180 - m_graphics->charAngle)), m_graphics->world, m_graphics->charAngle);
		}
		else{
			m_graphics->objects[1]->translateModel(1, btVector3(-.2*cos(180 - m_graphics->charAngle), 0 ,-0.2*sin(180 - m_graphics->charAngle)), m_graphics->world, m_graphics->charAngle);
		}
	}


        else if(m_graphics->moveAngle < 3.14*4 && m_graphics->moveAngle > 3.14*2)
	{
		if(m_graphics->charAngle < 3.14)
		{
			m_graphics->objects[1]->translateModel(1, btVector3(.2*cos(180+  m_graphics->charAngle), 0 ,0.2*sin(180+ m_graphics->charAngle)), m_graphics->world, m_graphics->charAngle);
		}
		else{
			m_graphics->objects[1]->translateModel(1, btVector3(-.2*cos(180+ m_graphics->charAngle), 0 ,-0.2*sin(180+ m_graphics->charAngle)), m_graphics->world, m_graphics->charAngle);
		}
	}

	//DONe/////////////////////////////////////////////////////////////////////
	else if(m_graphics->moveAngle > 3.14*6.5)
	{
		if(m_graphics->charAngle > 3.14)
		{
			m_graphics->objects[1]->translateModel(1, btVector3(.2*cos(90- m_graphics->charAngle), 0 ,0.2*sin(90-  m_graphics->charAngle)), m_graphics->world, m_graphics->charAngle);
		}
		else
		{
			m_graphics->objects[1]->translateModel(1, btVector3(-.2*cos(90- m_graphics->charAngle), 0 ,-0.2*sin(90-  m_graphics->charAngle)), m_graphics->world, m_graphics->charAngle);
		}
	}
        //m_graphics->objects[1]->translateModel(1, 2, m_graphics->world, m_graphics->charAngle);
        m_graphics->getCamera()->UpdateCamera(m_graphics->objects[1]->GetModel(), m_graphics->objects[2]->GetModel());
        break;
      case SDLK_s://Move Camera Backward
        if(3.14*6.5 > m_graphics->moveAngle && (3.14)*4 < m_graphics->moveAngle)
	{
		if(m_graphics->charAngle > 3.14)
		{
        		m_graphics->objects[1]->translateModel(1, btVector3(.2*cos(270 - m_graphics->charAngle), 0 ,0.2*sin(270 - m_graphics->charAngle)), m_graphics->world, m_graphics->charAngle);
		}
		else{
			m_graphics->objects[1]->translateModel(1, btVector3(-.2*cos(270 - m_graphics->charAngle), 0 ,-0.2*sin(270 - m_graphics->charAngle)), m_graphics->world, m_graphics->charAngle);
		}
	}
	else if(m_graphics->moveAngle == 3.14*2)
	{
		m_graphics->objects[1]->translateModel(1, btVector3(0.2, 0 ,0), m_graphics->world, m_graphics->charAngle);
	}

        else if(m_graphics->moveAngle < 3.14*4 && m_graphics->moveAngle > 3.14*2)
	{
		if(m_graphics->charAngle < 3.14)
		{
			m_graphics->objects[1]->translateModel(1, btVector3(-.2*cos(180+  m_graphics->charAngle), 0 ,-0.2*sin(180+ m_graphics->charAngle)), m_graphics->world, m_graphics->charAngle);
		}
		else{
			m_graphics->objects[1]->translateModel(1, btVector3(.2*cos(180+ m_graphics->charAngle), 0 ,0.2*sin(180+ m_graphics->charAngle)), m_graphics->world, m_graphics->charAngle);
		}
	}
	//DONe////////////////////////////////////////////////////////////////////
	else if(m_graphics->moveAngle < 3.14*2 && m_graphics->moveAngle > 0)
	{
		if(m_graphics->charAngle > 3.14)
		{
			m_graphics->objects[1]->translateModel(1, btVector3(-.2*cos(180 - m_graphics->charAngle), 0 ,-0.2*sin(180 - m_graphics->charAngle)), m_graphics->world, m_graphics->charAngle);
		}
		else{
			m_graphics->objects[1]->translateModel(1, btVector3(.2*cos(180 - m_graphics->charAngle), 0 ,0.2*sin(180 - m_graphics->charAngle)), m_graphics->world, m_graphics->charAngle);
		}
	}

	//0-90
	else if(m_graphics->moveAngle > 3.14*6.5)
	{
                if(m_graphics->charAngle > 3.14)
		{
			m_graphics->objects[1]->translateModel(1, btVector3(-.2*cos(90- m_graphics->charAngle), 0 ,-0.2*sin(90-  m_graphics->charAngle)), m_graphics->world, m_graphics->charAngle);
		}
		else
		{
			m_graphics->objects[1]->translateModel(1, btVector3(.2*cos(90- m_graphics->charAngle), 0 ,0.2*sin(90-  m_graphics->charAngle)), m_graphics->world, m_graphics->charAngle);
		}
	}
	m_graphics->getCamera()->UpdateCamera(m_graphics->objects[1]->GetModel(), m_graphics->objects[2]->GetModel());
        break;
      case SDLK_v:
        m_graphics->setShader('v');
        break;
      case SDLK_f:
        m_graphics->setShader('f');
        break;
      case SDLK_g:
        if(m_graphics->ambControl <= .7)
        	m_graphics->ambControl += 0.2;
        break;
      case SDLK_b:
        if(m_graphics->ambControl >= -.2)
        	m_graphics->ambControl -= 0.2;
        break;
      case SDLK_h:
        if(m_graphics->shininessControl <= .8)
        	m_graphics->shininessControl += 0.2;
        break;
      case SDLK_n:
        if(m_graphics->shininessControl >= 0)
        	m_graphics->shininessControl -= 0.2;
        break;

      case SDLK_LEFT:
	if(m_graphics->moveAngle + .2 < (3.14 * 9))
	{
		m_graphics->charAngle += 0.05;
		m_graphics->moveAngle += .5;
	}
	else
	{
		m_graphics->charAngle = 0;
		m_graphics->moveAngle = 0;
	}
        m_graphics->objects[1]->rotateModel(1, btQuaternion(0, 0.05 ,0, 1), m_graphics->world, m_graphics->charAngle);
        m_graphics->getCamera()->UpdateCamera(m_graphics->objects[1]->GetModel(), m_graphics->objects[2]->GetModel());
        break;
      case SDLK_RIGHT:
	if(m_graphics->moveAngle - .2 > 0)
	{
		m_graphics->charAngle -= 0.05;
		m_graphics->moveAngle -= .5;
	}
	else
	{
		m_graphics->charAngle = (3.14 * 2);
		m_graphics->moveAngle = (3.14 * 9);
	}
        m_graphics->objects[1]->rotateModel(1, btQuaternion(0, -0.05 ,0, 1), m_graphics->world, m_graphics->charAngle);
        m_graphics->getCamera()->UpdateCamera(m_graphics->objects[1]->GetModel(), m_graphics->objects[2]->GetModel());
        break;

      if (m_event.key.keysym.sym == SDLK_ESCAPE)
      {
        m_running = false;
      }
    }
  }
}
void Engine::Mouse()
{
  // int width, height, newMouseX, newMouseY;
  // SDL_GetWindowSize(m_window->getgWindow(), &width, &height);
  // SDL_GetMouseState(&newMouseX, &newMouseY);
  // if(newMouseX < 0 || newMouseX > width || newMouseY < 0 || newMouseY > height)
  // {
  //   m_window->getgWindow()->SDL_WarpMouse(width/2, height/2);
  //   return;
  // }
  // if(m_event.type == SDL_MOUSEMOTION)
  // {
  //   float x = (float)newMouseX;
  //   float y = (float)newMouseY;
  //   m_graphics->getCamera()->UpdateCamera(glm::vec3(0.0, 0.0, 0.0), glm::vec3(mouseX-x, 0.0, 0.0));
  //   mouseX = x;
  // }
}

unsigned int Engine::getDT()
{
  long long TimeNowMillis = GetCurrentTimeMillis();
  assert(TimeNowMillis >= m_currentTimeMillis);
  unsigned int DeltaTimeMillis = (unsigned int)(TimeNowMillis - m_currentTimeMillis);
  m_currentTimeMillis = TimeNowMillis;
  return DeltaTimeMillis;
}

long long Engine::GetCurrentTimeMillis()
{
  timeval t;
  gettimeofday(&t, NULL);
  long long ret = t.tv_sec * 1000 + t.tv_usec / 1000;
  return ret;
}

Window* Engine::getWindow(){return m_window;}
