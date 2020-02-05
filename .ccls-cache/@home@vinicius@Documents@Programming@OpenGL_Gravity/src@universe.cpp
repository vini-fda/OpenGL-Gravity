#include "universe.h"
#include <glm/gtx/norm.hpp>
#include <glm/gtc/matrix_transform.hpp>


Universe::Universe(unsigned int width, unsigned int height)
{
  this->width = width;
  this->height = height;
}
Universe::~Universe()
{
  delete p;
}
void Universe::Init()
{
  projection = glm::ortho(0.0f, (float)width, (float)height, 0.0f, -1.0f, 1.0f);
  p = new Planet;
}
void Universe::ProcessInput(GLFWwindow *window)
{
  if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
      glfwSetWindowShouldClose(window, true);
  if(glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
    zoom *= 1.01;
  if(glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
    zoom /= 1.01;
  zoom = glm::clamp<float>(zoom, 0.1, 100000.0);
  //
  constexpr float speed = 0.001;
  glm::vec2 motion = glm::vec2(0.0, 0.0);
  if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    motion += glm::vec2(0.0, 1.0);
  if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    motion += glm::vec2(0.0, -1.0);
  if(glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
    motion += glm::vec2(-1.0, 0.0);
  if(glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    motion += glm::vec2(1.0, 0.0);
  if(glm::length2(motion) > 0.0)
    motion = speed * glm::normalize(motion);
}
void Universe::Update(float dt)
{

}
void Universe::Render()
{
  p->shader.setMat4("view", view);
  p->shader.setMat4("projection", projection);
  p->draw();
}
void Universe::UpdateProjection(float width, float height)
{
  projection = glm::ortho(0.0f, (float)width, (float)height, 0.0f, -1.0f, 1.0f);
}
