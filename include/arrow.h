#ifndef ARROW_H
#define ARROW_H

#include <glm/glm.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shader.h"

class Arrow
{
public:
  Arrow();

  glm::dvec2 position{glm::dvec2(0.0,0.0)};
  glm::dvec2 vector{glm::dvec2(0,0)};

  glm::mat4 projection;
  glm::mat4 view;

  void draw();
  Shader shader;
private:
  static const int NUM_VERTICES = 6;
  unsigned int VAO;
  unsigned int VBO;
  float vertices[3*NUM_VERTICES] = {
     0.0f,  0.0f, 0.0f,
     0.75f,  0.0f, 0.0f,
     0.75f, 0.144f, 0.0f,
     1.0f,  0.0f, 0.0f,
     0.75f, -0.144f, 0.0f,
     0.75f,  0.0f, 0.0f
  };
};

#endif
