
#include "engine.h"
#include <btBulletDynamicsCommon.h>

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
  if(m_event.type == SDL_QUIT)
  {
    m_running = false;
  }
  else if (m_event.type == SDL_KEYDOWN)
  {
    switch(m_event.key.keysym.sym)
    {
    case SDLK_LEFT:
      m_graphics->objects[2]->translateModel(btVector3(.2,0,0), m_graphics->world);
      break;
    case SDLK_RIGHT:
      m_graphics->objects[2]->translateModel(btVector3(-.2,0,0), m_graphics->world);
      break;
    case SDLK_UP:
     m_graphics->objects[2]->translateModel(btVector3(0,0,.2), m_graphics->world);
      break;
    case SDLK_DOWN:
     m_graphics->objects[2]->translateModel(btVector3(0,0,-.2), m_graphics->world);
      break;
    case SDLK_v:
      m_graphics->setShader('v');
      break;
    case SDLK_f:
      m_graphics->setShader('f');
      break;

    //Brightness
    case SDLK_a:
      if(m_graphics->shineAmb <= 1.5)
        m_graphics->shineAmb += 0.02;
      break;
    case SDLK_z:
      if(m_graphics->shineAmb >= -1.5)
        m_graphics->shineAmb -= 0.02;
      break;
    case SDLK_s:
      if(m_graphics->shineSpec <= 1.5)
      m_graphics->shineSpec += 0.02;
//printf("%g", m_graphics->shineSpec);
      break;
    case SDLK_x:
      if(m_graphics->shineSpec >= -1.5)
        m_graphics->shineSpec -= 0.02;
//printf("%g", m_graphics->shineSpec);
    //Spotlight size
    case SDLK_i:
      m_graphics->spotAperture += 2;
    case SDLK_o:
      m_graphics->spotAperture -= 2;
      break;
    }
    if (m_event.key.keysym.sym == SDLK_ESCAPE)
    {
      m_running = false;
    }
  }
  /*else if(m_event.button.type == SDL_MOUSEBUTTONDOWN)
  {
    //handle button presses here
    switch(m_event.button.button)
    {
    case SDL_BUTTON_MIDDLE:
      m_graphics->m_cube->setTranslate('V');
      m_graphics->m_cube->setRotate('G');
      break;
    case SDL_BUTTON_RIGHT:
      m_graphics->m_cube->setTranslate('F');
      m_graphics->m_cube->setRotate('B');
      break;
    case SDL_BUTTON_LEFT:
      m_graphics->m_cube->setTranslate('R');
      m_graphics->m_cube->setRotate('T');
      break;
    }
  }*/
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
