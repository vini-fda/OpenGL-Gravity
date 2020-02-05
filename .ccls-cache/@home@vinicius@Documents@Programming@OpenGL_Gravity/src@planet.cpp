#include "planet.h"
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;
Planet::Planet()
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
                    "./shaders/planet_shader.fs");
  //VBO & VAO
  glGenBuffers(1, &VBO);
  glGenVertexArrays(1, &VAO);
  //Initialization
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), NULL);
  glEnableVertexAttribArray(0);
}
Planet::Planet(dvec2 position, dvec2 velocity, double radius, double mass) : Planet()
{
  this->position = position;
  this->velocity = velocity;
  this->radius = radius;
  this->mass = mass;
}
void Planet::draw()
{
  shader.use();
  mat4 model = mat4(1.0f);
  model = translate(model, vec3(position, 0.0));
  model = scale(model, vec3(radius, radius, 1.0));
  shader.setMat4("model", model);
  glBindVertexArray(VAO);
  glDrawArrays(GL_LINE_LOOP, 0, NUM_VERTICES);
}
void Planet::processInput()
{

}
