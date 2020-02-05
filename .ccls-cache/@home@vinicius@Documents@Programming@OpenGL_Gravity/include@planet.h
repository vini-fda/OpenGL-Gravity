#ifndef PLANET_H
#define PLANET_H

#include <glm/glm.hpp>
#include "shader.h"

class Planet
{
public:
  Planet();
  Planet(glm::dvec2 position, glm::dvec2 velocity, double radius, double mass);
  glm::dvec2 force{glm::dvec2(0,0)};
  glm::dvec2 velocity{glm::dvec2(0,0)};
  glm::dvec2 position{glm::dvec2(200.0,10.0)};
  double radius{1.0};
  double mass{1.0};
  void draw();
  void processInput();
  Shader shader;
private:
  static const int NUM_VERTICES = 32;
  float vertices[3*NUM_VERTICES];
  unsigned int VAO;
  unsigned int VBO;
};

#endif
