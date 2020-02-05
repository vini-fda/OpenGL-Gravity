#include <iostream>
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shader.h"
#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/constants.hpp>

static float zoom = 1.0;
static glm::vec2 center =  glm::vec2(0.0, 0.0);

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if(glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
      zoom *= 1.01;
    if(glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
      zoom /= 1.01;
    zoom = glm::clamp<float>(zoom, 0.1, 100000.0);
    //
    const float speed = 0.001;
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
    center += motion/zoom;

}

int main(int argc, char const *argv[]) {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


  //Load GLFW window
  GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
  if (window == NULL)
  {
      std::cout << "Failed to create GLFW window" << std::endl;
      glfwTerminate();
      return -1;
  }
  glfwMakeContextCurrent(window);


  //Load GLAD
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
      std::cout << "Failed to initialize GLAD" << std::endl;
      return -1;
  }
  glViewport(0, 0, 800, 600);

  //---------------------------Hello----------------------------------------
  //Whenever the window resizes (callback)
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  //VBO
  /*
  float vertices[] = {
       0.866f, -0.5f, 0.0f,
       0.000f,  1.0f, 0.0f,
      -0.866f, -0.5f, 0.0f,
  };*/
  //float* vertices = rectangle(0.0f,0.0f,0.4f,0.4f);
  float vertices[] = {
     1.0f,  1.0f, 0.0f,  // top right
     1.0f, -1.0f, 0.0f,  // bottom right
    -1.0f, -1.0f, 0.0f,  // bottom left
    -1.0f,  1.0f, 0.0f   // top left
  };
  float rect_colors[] = {
    1.0f, 1.0f,  1.0f,  // top right
    1.0f, 0.0f,  0.0f,  // bottom right
    0.0f, 1.0f,  0.0f,  // bottom left
    0.0f, 0.0f,  1.0f   // top left
  };
  float rect_tex[] = {
    1.0f, 1.0f,
    1.0f, 0.0f,
    0.0f, 0.0f,
    0.0f, 1.0f
  };
  //Texture
  unsigned int texture;
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_1D, texture);
  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  //Texture data
  int width, height, nrChannels;
  unsigned char *data = stbi_load("images/palette.png", &width, &height, &nrChannels, 0);
  if (data)
  {
      printf("%i\n", width);
      glTexImage1D(GL_TEXTURE_1D, 0, GL_RGB, width, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
      //glGenerateMipmap(GL_TEXTURE_1D);
  }
  else
  {
      std::cout << "Failed to load texture" << std::endl;
  }
  stbi_image_free(data);
  //Texture position VBO
  unsigned int texture_VBO;
  glGenBuffers(1, &texture_VBO);
  glBindBuffer(GL_ARRAY_BUFFER, texture_VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(rect_tex), rect_tex, GL_STATIC_DRAW);
  //Vertex position VBO
  unsigned int VBO,rect_colors_VBO;
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  //Color VBO
  glGenBuffers(1, &rect_colors_VBO);
  glBindBuffer(GL_ARRAY_BUFFER, rect_colors_VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(rect_colors), rect_colors, GL_STATIC_DRAW);

  //shader
  Shader hello_shader = Shader("./shaders/hello_shader.vs",
                               "./shaders/julia.fs");
  //EBO
  unsigned int indices[] = {
    0,1,3,
    1,2,3
  };
  unsigned int EBO;
  glGenBuffers(1, &EBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  //VAO
  unsigned int VAO;
  glGenVertexArrays(1, &VAO);

  // ..:: Initialization code (done once (unless your object frequently changes)) :: ..
  // 1. bind Vertex Array Object
  glBindVertexArray(VAO);
  // 2. copy our vertices array in a buffer for OpenGL to use
  // 3. then set our vertex attributes pointers
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
  glBindBuffer(GL_ARRAY_BUFFER, rect_colors_VBO);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
  glBindBuffer(GL_ARRAY_BUFFER, texture_VBO);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, NULL);
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glEnableVertexAttribArray(2);


  //hello_shader.use(); // don't forget to activate the shader before setting uniforms!

  //glm::mat4 trans = glm::mat4(1.0f);
  //trans = glm::rotate(trans, glm::half_pi<float>(), glm::vec3(0.0, 0.0, 1.0));
  //trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));

  unsigned int transformLoc = glGetUniformLocation(hello_shader.ID, "transform");
  //glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));

  //Render loop
  while(!glfwWindowShouldClose(window))
  {
      //input
      processInput(window);
      //rendering
      glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT);
      //draw
      hello_shader.use();
      hello_shader.setFloat("time", glfwGetTime());
      //hello_shader.setFloat("zoom", zoom);
      //hello_shader.setVec2("center", center);


      glm::mat4 trans = glm::mat4(1.0f);
      //trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0, 0.0, 1.0));
      trans = glm::translate(trans, glm::vec3(center[0],center[1],0.0));
      trans = glm::scale(trans, glm::vec3(1.0/zoom));
      glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
      glBindTexture(GL_TEXTURE_1D, texture);
      glBindVertexArray(VAO);
      glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
      //check and call events and swap buffers
      glfwSwapBuffers(window);
      glfwPollEvents();
  }



  glfwTerminate();
  return 0;
}
