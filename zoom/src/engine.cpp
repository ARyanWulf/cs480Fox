
#include "engine.h"

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

  // No errors
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
  std::string planNames[9] = {"Mercury", "Venus", "Earth", "Mars", "Jupiter", "Saturn", "Uranus", "Neptune", "Pluto"};
  if(m_event.type == SDL_QUIT)
  {
    m_running = false;
  }
  else if (m_event.type == SDL_KEYDOWN)
  {
    // handle key down events here
    switch(m_event.key.keysym.sym)
    {
      case SDLK_ESCAPE:
        m_running = false;
        break;
      case SDLK_LEFT:
	if(m_graphics->getPlanCursor() > 0)
	{
		m_graphics->decCursor();
                cout << m_graphics->m_planets[m_graphics->getPlanCursor()].name << " " << m_graphics->getPlanCursor() << endl;
		if(m_graphics->m_planets[m_graphics->getPlanCursor() - 1].name != "Uranus" && m_graphics->m_planets[m_graphics->getPlanCursor() - 1].name != "Neptune" && m_graphics->m_planets[m_graphics->getPlanCursor() - 1].name != "Saturn"){
			m_graphics->resetZoom(glm::vec3{0.0, 8.0, -27.0});
		}
		else if(m_graphics->m_planets[m_graphics->getPlanCursor()].name == 
"Jupiter")
		{
			m_graphics->resetZoom(glm::vec3{0.0, 8.0, -500.0});
		}
		else{
			m_graphics->resetZoom(glm::vec3{0.0, 8.0, -300.0});
		}
	}
        else
	{
		m_graphics->resetCursor(1);
		m_graphics->resetZoom(glm::vec3{0.0, 18.0, -500.0});
	}
        break;
      case SDLK_RIGHT:
	if(m_graphics->getPlanCursor() < m_graphics->getNumPlan() - 1)
	{
		m_graphics->incCursor();
		if(m_graphics->m_planets[m_graphics->getPlanCursor() - 1].name != "Uranus" && m_graphics->m_planets[m_graphics->getPlanCursor() - 1].name != "Neptune" && m_graphics->m_planets[m_graphics->getPlanCursor() - 1].name != "Saturn"){
			m_graphics->resetZoom(glm::vec3{0.0, 8.0, -27.0});
		}
		else if(m_graphics->m_planets[m_graphics->getPlanCursor()].name == 
"Jupiter")
		{
			m_graphics->resetZoom(glm::vec3{0.0, 8.0, -500.0});
		}
		else{
			m_graphics->resetZoom(glm::vec3{0.0, 8.0, -300.0});
		}
	}
        else if(m_graphics->getPlanCursor() == m_graphics->getNumPlan() - 1)
	{
		m_graphics->incCursor();
		m_graphics->resetZoom(glm::vec3{0.0, 18.0, -500.0});
	}
        else if(m_graphics->getPlanCursor() == m_graphics->getNumPlan())
	{
		m_graphics->resetCursor(0);
		m_graphics->resetZoom(glm::vec3{0.0, 8.0, -27.0});
	}
        break;
    }
   }
    else if (m_event.type == SDL_MOUSEWHEEL)
  {
    // handle mouse wheel
    switch(m_event.wheel.y) 
    {
      case 1: //Scroll Up
        m_graphics->zoomIn(10);
        break;
      case -1: //Scroll Down
        m_graphics->zoomOut(10);
        break;
    }
  }
  else if(m_event.button.type == SDL_MOUSEBUTTONDOWN)
  {
    switch(m_event.button.button)
    {
      case SDL_BUTTON_LEFT:
        m_graphics->setCubeStatusT('f');
        m_graphics->setCubeStatusR('r');
        break;
      case SDL_BUTTON_MIDDLE:
        m_graphics->setCubeStatusT('s');
        m_graphics->setCubeStatusR('p');
        break;
      case SDL_BUTTON_RIGHT:
        m_graphics->setCubeStatusT('b');
        m_graphics->setCubeStatusR('n');
        break;
    }
  }
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
