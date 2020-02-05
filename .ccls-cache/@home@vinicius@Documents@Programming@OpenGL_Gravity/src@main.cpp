#include <iostream>
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "universe.h"

const int defaultWidth = 800;
const int defaultHeight = 600;
Universe universe(defaultWidth, defaultHeight);

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    universe.UpdateProjection((float)width, (float)height);
}


int main(int argc, char const *argv[]) {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  //Load GLFW window
  GLFWwindow* window = glfwCreateWindow(defaultWidth, defaultHeight, "Hello World!", NULL, NULL);
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
  glViewport(0, 0, defaultWidth, defaultHeight);

  //Whenever the window resizes (callback)
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  //Initialize universe
  universe.Init();
  //Render loop
  while(!glfwWindowShouldClose(window))
  {
      //input
      universe.ProcessInput(window);
      //rendering
      glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT);
      //draw
      universe.Render();
      //check and call events and swap buffers
      glfwSwapBuffers(window);
      glfwPollEvents();
  }
  glfwTerminate();
  return 0;
}
