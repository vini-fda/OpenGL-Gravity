#ifndef UNIVERSE_H
#define UNIVERSE_H
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include "planet.h"
#include "camera.h"

class Universe
{
public:
  Universe(unsigned int width, unsigned int height);
  ~Universe();

  //float zoom = 1.0;
  unsigned int width;
  unsigned int height;
  glm::mat4 projection;
  glm::mat4 view;
  std::vector<Planet> planets;
  Camera mainCamera;

  void Init();
  void ProcessInput(GLFWwindow *window);
  void Update(float dt);
  void Render();
  void UpdateProjection(float width, float height);
};

#endif
