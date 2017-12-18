#include "shader.h"
#include <iostream>
#include <fstream>

Shader::Shader()
{
  m_shaderProg = 0;
}

Shader::~Shader()
{
  for (std::vector<GLuint>::iterator it = m_shaderObjList.begin() ; it != m_shaderObjList.end() ; it++)
  {
    glDeleteShader(*it);
  }

  if (m_shaderProg != 0)
  {
    glDeleteProgram(m_shaderProg);
    m_shaderProg = 0;
  }
}

bool Shader::Initialize()
{
  m_shaderProg = glCreateProgram();

  if (m_shaderProg == 0) 
  {
    std::cerr << "Error creating shader program\n";
    return false;
  }

  shading = 'v';

  return true;
}

void Shader::setShader(char shaderFile){
   shading = shaderFile;
}

// Use this method to add shaders to the program. When finished - call finalize()
bool Shader::AddShader(GLenum ShaderType)
{
  std::string s, fileName, temp;
  bool fileOpen = false;
  std::ifstream shader;

  if(ShaderType == GL_VERTEX_SHADER)
  {
     std::cout << "shader " << std::endl;
     if(shading == 'v')
	fileName = "vl.txt";
     else if (shading == 'f' )
        fileName = "vf.txt";
     else{
        std::cout << "ERROR: INVALID SHADER" << std::endl;
        return false;
     }

 std::cout << fileName << std::endl;
      shader.open(fileName);
      if(shader.good())
      {
      	fileOpen = true;
      }
      else
      {
        std::cout << "File not present" << std::endl;
      }
 
    
      while(shader.good())
      {
        std::getline(shader, temp);
        s += temp;
        s += '\n';
      }
  }
  else if(ShaderType == GL_FRAGMENT_SHADER)
  {
    while(fileOpen == false)
    {
      if(shading == 'v')
	 fileName = "f.txt";
      else if (shading == 'f' )
         fileName = "fl.txt";
      else{
         std::cout << "ERROR: INVALID SHADER" << std::endl;
         return false;
      }
      shader.open(fileName);

      if(shader.good())
      {
      	fileOpen = true;
      }
      else
      {
        std::cout << "File not present" << std::endl;
      }
    }

    while(shader.good())
    {
      std::getline(shader, temp);
      s += temp;
      s += '\n';
    }
  }

  shader.clear();
  shader.close();

  GLuint ShaderObj = glCreateShader(ShaderType);

  if (ShaderObj == 0) 
  {
    std::cerr << "Error creating shader type " << ShaderType << std::endl;
    return false;
  }

  // Save the shader object - will be deleted in the destructor
  m_shaderObjList.push_back(ShaderObj);

  const GLchar* p[1];
  p[0] = s.c_str();
  GLint Lengths[1] = { (GLint)s.size() };

  glShaderSource(ShaderObj, 1, p, Lengths);

  glCompileShader(ShaderObj);

  GLint success;
  glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);

  if (!success) 
  {
    GLchar InfoLog[1024];
    glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
    std::cerr << "Error compiling: " << InfoLog << std::endl;
    return false;
  }

  glAttachShader(m_shaderProg, ShaderObj);

  return true;
}


// After all the shaders have been added to the program call this function
// to link and validate the program.
bool Shader::Finalize()
{
  GLint Success = 0;
  GLchar ErrorLog[1024] = { 0 };

  glLinkProgram(m_shaderProg);

  glGetProgramiv(m_shaderProg, GL_LINK_STATUS, &Success);
  if (Success == 0)
  {
    glGetProgramInfoLog(m_shaderProg, sizeof(ErrorLog), NULL, ErrorLog);
    std::cerr << "Error linking shader program: " << ErrorLog << std::endl;
    return false;
  }

  glValidateProgram(m_shaderProg);
  glGetProgramiv(m_shaderProg, GL_VALIDATE_STATUS, &Success);
  if (!Success)
  {
    glGetProgramInfoLog(m_shaderProg, sizeof(ErrorLog), NULL, ErrorLog);
    std::cerr << "Invalid shader program: " << ErrorLog << std::endl;
    return false;
  }

  // Delete the intermediate shader objects that have been added to the program
  for (std::vector<GLuint>::iterator it = m_shaderObjList.begin(); it != m_shaderObjList.end(); it++)
  {
    glDeleteShader(*it);
  }

  m_shaderObjList.clear();

  return true;
}


void Shader::Enable()
{
    glUseProgram(m_shaderProg);
}


GLint Shader::GetUniformLocation(const char* pUniformName)
{
    GLuint Location = glGetUniformLocation(m_shaderProg, pUniformName);

    if (Location == INVALID_UNIFORM_LOCATION) {
        fprintf(stderr, "Warning! Unable to get the location of uniform '%s'\n", pUniformName);
    }

    return Location;
}
