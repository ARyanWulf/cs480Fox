#include "camera.h"

Camera::Camera()
{

}

Camera::~Camera()
{

}

bool Camera::Initialize(int w, int h)
{
  //--Init the view and projection matrices
  //  if you will be having a moving camera the view matrix will need to more dynamic
  //  ...Like you should update it before you render more dynamic
  //  for this project having them static will be fine

  cPostition = glm::vec3(0, 180.0, 0.0);
  cDirection = glm::vec3(2.0, 0.0, 0.0);
  cUp = glm::vec3(0.0, 1.0, 0.0);

  view = glm::lookAt( cPostition, //Eye Position
                      cDirection, //Focus point
                      cUp); //Positive Y is up

  projection = glm::perspective( 45.0f, //the FoV typically 90 degrees is good which is what this is set to
                                 float(w)/float(h), //Aspect Ratio, so Circles stay Circular
                                 0.01f, //Distance to the near plane, normally a small value like this
                                 200.0f); //Distance to the far plane,
  return true;
}

//Camera Movement still independent of look direction
glm::mat4 Camera::UpdateCamera(glm::mat4 model, glm::mat4 directionModel)
{
	glm::vec3 modelPos(model[3]);
        glm::vec3 modelDir(directionModel[3]);
	cDirection = modelDir;
 	cPostition = modelPos;
	//cDirection.y = modelPos.y + cos(angle);
  view = glm::lookAt( cPostition, //Eye Position
                      cDirection, //Focus point
                      cUp); //Positive Y is up
}
/*
  Two invisible Cubes: one on camera, one on the focus point
  Keep track of the angle of the cube and use orbit code to rotate cDirection
    Use cosine of the angle to control the movement of the cube
*/
// void Camera::Update(glm::mat4 model, char type)
// {
//
// 	glm::vec3 modelPos(model[3]);
// 	glm::vec3 cameraPosition = modelPos + offset;
//
// 	view = glm::lookAt( glm::vec3(cameraPosition.x, cameraPosition.y, cameraPosition.z), //Eye Position
//                       modelPos, //Focus point
//                       glm::vec3(0.0, 1.0, 0.0));
//
// }

glm::mat4 Camera::GetProjection()
{
  return projection;
}

glm::mat4 Camera::GetView()
{
  return view;
}
