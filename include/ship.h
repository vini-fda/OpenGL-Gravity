#ifndef SHIP_H
#define SHIP_H

#include <glm/glm.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shader.h"
#include "arrow.h"

class Ship
{
public:
  Ship();
  Ship(glm::dvec2 position, glm::dvec2 velocity, double radius, double mass);

  glm::dvec2 thrust{glm::dvec2(0,0)};
  glm::dvec2 gravForce{glm::dvec2(0,0)};
  glm::dvec2 velocity{glm::dvec2(0,0)};
  glm::dvec2 position{glm::dvec2(0,0)};
  double torque = 0;
  double angularVelocity = 0;
  glm::dvec2 direction{glm::dvec2(1,0)};
  double radius{1.0};
  double mass{1.0};

  glm::mat4 projection;
  glm::mat4 view;

  glm::dvec2 getAppliedForce();
  void draw();
  void processInput(GLFWwindow *window);
  Shader shader;
private:
  static const int NUM_VERTICES = 32;
  float vertices[3*NUM_VERTICES];
  unsigned int VAO;
  unsigned int VBO;
  Arrow forceArrow;
  Arrow velocityArrow;
  Arrow directionArrow;
};

#endif
