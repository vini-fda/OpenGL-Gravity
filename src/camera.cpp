#include "camera.h"
#include <glm/gtc/matrix_transform.hpp>
Camera::Camera(){}
Camera::Camera(float width, float height) : viewWidth(width), viewHeight(height)
{
}

void Camera::UpdateView(float width, float height)
{
  this->viewWidth = width;
  this->viewHeight = height;
}
glm::mat4 Camera::getTransform()
{
  glm::mat4 transform(1.0f);
  transform = glm::translate(transform,   glm::vec3(viewWidth/2, viewHeight/2, 0));
  transform = glm::scale(transform, glm::vec3(zoom, zoom, 1));
  transform = glm::translate(transform, - glm::vec3(viewWidth/2, viewHeight/2, 0));
  transform = glm::scale(transform, glm::vec3(1.0, -1.0, 0));
  transform = glm::translate(transform, glm::vec3(-center, 0));

  return transform;
}
