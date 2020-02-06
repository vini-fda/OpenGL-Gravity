#include "arrow.h"

#include <glm/gtx/norm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;


Arrow::Arrow()
{
  //Shader set up
  shader = Shader("./shaders/planet_shader.vs",
                    "./shaders/arrow_shader.fs");
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

void Arrow::draw()
{
  shader.setMat4("view", view);
  shader.setMat4("projection", projection);
  shader.use();
  mat4 trans = mat4(1.0f);
  trans = translate(trans, vec3(position, 0.0));
  mat4 model = mat4(vector.x,vector.y,0,0,
                    -vector.y, vector.x,0,0,
                                   0,0,1,0,
                                   0,0,0,1);
  shader.setMat4("model", trans * model);
  glBindVertexArray(VAO);
  glDrawArrays(GL_LINE_LOOP, 0, NUM_VERTICES);
}
