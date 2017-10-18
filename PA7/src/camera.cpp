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
  cameraPosition = {0.0, 8.8, -16.0};
  offset = {0.0, 8.0, -16.0};
  view = glm::lookAt( glm::vec3(cameraPosition.x, cameraPosition.y, cameraPosition.z), //Eye Position
                      glm::vec3(0.0, 0.0, 0.0), //Focus point
                      glm::vec3(0.0, 1.0, 0.0)); //Positive Y is up

  projection = glm::perspective( 45.0f, //the FoV typically 90 degrees is good which is what this is set to
                                 float(w)/float(h), //Aspect Ratio, so Circles stay Circular
                                 0.01f, //Distance to the near plane, normally a small value like this
                                 10000.0f); //Distance to the far plane, 
  return true;
}

glm::mat4 Camera::GetProjection()
{
  return projection;
}

glm::mat4 Camera::GetView()
{
  return view;
}

void Camera::zoomIn(int add){
	if(offset.z < -9){
		offset.z += add;
		offset.y -= (add * .5);
	}
}

void Camera::zoomOut(int add){
	//if(offset.z > -1000){
		offset.z -= add;
		offset.y += (add * .5);
	//}
}

void Camera::resetOff(glm::vec3 newV){
	offset = newV;
}

void Camera::Update(glm::mat4 model, char type)
{

	glm::vec3 modelPos(model[3]);
	cameraPosition = modelPos + offset;

	view = glm::lookAt( glm::vec3(cameraPosition.x, cameraPosition.y, cameraPosition.z), //Eye Position
                      modelPos, //Focus point
                      glm::vec3(0.0, 1.0, 0.0));

}

