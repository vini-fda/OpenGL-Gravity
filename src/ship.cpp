#include "ship.h"

#include <glm/gtx/norm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;


Ship::Ship()
{
  //Vertices set up
  for (size_t i = 0; i < NUM_VERTICES; i++) {
    float theta = 2*i*pi<float>()/NUM_VERTICES;
    vertices[3*i] = cos(theta);
    vertices[3*i+1] = sin(theta);
    vertices[3*i+2] = 0.0;
  }
  //Shader set up
  shader = Shader("./shaders/planet_shader.vs",
                    "./shaders/ship_shader.fs");
  //VBO & VAO
  glGenBuffers(1, &VBO);
  glGenVertexArrays(1, &VAO);
  //Initialization
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), NULL);
  glEnableVertexAttribArray(0);
  //
  //directionArrow.color = glm::vec3(0.5, 1.0, 0.5);
}
Ship::Ship(dvec2 position, dvec2 velocity, double radius, double mass) : Ship()
{
  this->position = position;
  this->velocity = velocity;
  this->radius = radius;
  this->mass = mass;
}
glm::dvec2 Ship::getAppliedForce()
{
  return gravForce + thrust;
}
void Ship::draw()
{
  shader.setMat4("projection", projection);
  shader.setMat4("view", view);
  shader.use();
  mat4 model = mat4(1.0f);
  model = translate(model, vec3(position, 0.0));
  model = scale(model, vec3(radius, radius, 1.0));
  shader.setMat4("model", model);
  glBindVertexArray(VAO);
  glDrawArrays(GL_LINE_LOOP, 0, NUM_VERTICES);
  //Draw children
  forceArrow.position = position;
  forceArrow.vector = getAppliedForce();
  forceArrow.projection = projection;
  forceArrow.view = view;
  forceArrow.draw();
  velocityArrow.position = position;
  velocityArrow.vector = velocity;
  velocityArrow.projection = projection;
  velocityArrow.view = view;
  velocityArrow.draw();
  directionArrow.position = position;
  directionArrow.vector = 10.0 * direction;
  directionArrow.projection = projection;
  directionArrow.view = view;
  directionArrow.draw();
}
void Ship::processInput(GLFWwindow *window)
{
  constexpr double maxthrust = 200.0;
  constexpr double maxtorque = 10.0;
  torque = 0;
  thrust = glm::dvec2(0.0, 0.0);
  if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    torque += maxtorque;
  if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    torque -= maxtorque;
  if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    thrust += direction;
  if(glm::length2(thrust) > 0.0)
    thrust = maxthrust * glm::normalize(thrust);
}
