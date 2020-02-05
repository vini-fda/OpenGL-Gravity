#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
class Camera
{
public:
  Camera();
  Camera(float width, float height);
  void UpdateView(float width, float height);
  glm::vec2 center = glm::vec2(0,0);
  double zoom = 1.0;
  glm::mat4 getTransform();
private:
  float viewWidth, viewHeight;
};
#endif
