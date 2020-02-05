#include "universe.h"
#include <glm/gtx/norm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <tuple>
#include <chrono>
#include <functional>
#include <random>
#include <omp.h>

static const double G_CONST = 500.0;
static const int NUM_THREADS = 4;

Universe::Universe(unsigned int width, unsigned int height)
{
  this->width = width;
  this->height = height;
}

Universe::~Universe(){
  planets.clear();
}

static glm::dvec2 GravitationalForce(const glm::dvec2& distanceVec, const double& m1,const double& m2)
{
  //distanceVec should be (p2 - p1)
  constexpr double epsilon = 0.1;
  double distance = glm::length(distanceVec) + epsilon;
  return glm::dmat2(G_CONST*m1*m2/(distance*distance*distance)) * distanceVec;
}

static double CollisionTime(const glm::dvec2& p, const glm::dvec2& q, const glm::dvec2& relVelocity, double radiusSum, double dt)
{
  //Returns collision time between two circular bodies, in units of dt. (-1.0 if no collision)
  //p: first body's position
  //q: second body's position
  //solves for the smallest t such that at²+bt+c=0
  glm::dvec2 d = relVelocity * dt;
  double a = glm::dot(d,d);
  double b = 2*glm::dot(d,p-q);
  double c = glm::dot(p,p) + glm::dot(q,q) - 2*glm::dot(p,q) - (radiusSum*radiusSum);
  double discriminant = b*b-4*a*c;
  if(discriminant >= 0)
    return -0.5*(b+sqrt(discriminant))/a;
  else
    return -1.0;
}

static std::tuple<glm::dvec2, glm::dvec2> CollisionResponse(const glm::dvec2& distanceVec, const glm::dvec2& v1,const glm::dvec2& v2,const double& m1,const double& m2, double e)
{
  glm::dvec2 xAxis = glm::normalize(distanceVec);//points from p1 to p2 ( p1  ---->(+x) p2 )
  double v1x = glm::dot(v1, xAxis);
  double v2x = glm::dot(v2, xAxis);
  double mat[] = {
    (m1-e*m2)/(m1+m2), (m2+e*m2)/(m1+m2),
    (m1+e*m1)/(m1+m2), (m2-e*m1)/(m1+m2)
  };
  glm::dvec2 finalxSpeeds = glm::transpose(glm::make_mat2(mat))*glm::dvec2(v1x, v2x); //Need to transpose because glm interprets the input array as a column-based matrix
  glm::dvec2 u1 = (finalxSpeeds[0] - v1x)*xAxis + v1;
  glm::dvec2 u2 = (finalxSpeeds[1] - v2x)*xAxis + v2;

  return std::tuple<glm::dvec2, glm::dvec2>(u1, u2);
}

void Universe::Init()
{
  omp_set_num_threads(NUM_THREADS);

  mainCamera = Camera(width, height);
  mainCamera.center += glm::vec2(0, height);
  projection = glm::ortho(0.0f, (float)width, (float)height, 0.0f, -1.0f, 1.0f);

  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  std::default_random_engine generator(seed);
  std::uniform_real_distribution<float> pos_distribution(0.0,5000.0);
  auto random_real_pos = std::bind(pos_distribution, generator);
  std::uniform_real_distribution<float> vel_distribution(-20.0,20.0);
  auto random_real_vel = std::bind(vel_distribution, generator);
  std::uniform_real_distribution<float> rad_distribution(20.0,50.0);
  auto random_real_rad = std::bind(rad_distribution, generator);
  for (size_t i = 0; i < 400; i++) {
    Planet new_planet;
    new_planet.radius = random_real_rad();
    new_planet.mass = 2.5*new_planet.radius;//*new_planet.radius*new_planet.radius;
    new_planet.position = glm::vec2(random_real_pos(), random_real_pos());
    new_planet.velocity = glm::vec2(random_real_vel(), random_real_vel());
    planets.push_back(new_planet);
  }
  for (std::vector<Planet>::iterator it = planets.begin(); it != planets.end(); it++){
    it->force = glm::dvec2(0.0,0.0);
    for (std::vector<Planet>::iterator jt = planets.begin(); jt != planets.end(); jt++){
      if (it != jt){
        it->force += GravitationalForce(jt->position - it->position, it->mass, jt->mass);
      }
    }
  }
  /*
  Planet central_mass;
  central_mass.radius = 100.0;
  central_mass.mass = 5000.0;
  central_mass.position = glm::dvec2(0,0);
  planets.push_back(central_mass);
  Planet orbiter;
  double r = 170.0;
  orbiter.radius =1.0;
  orbiter.position = central_mass.position + glm::dvec2(0.0, r);
  orbiter.velocity = glm::dvec2(sqrt(G_CONST*central_mass.mass/r), 0.0);
  orbiter.force = GravitationalForce(central_mass.position - orbiter.position, orbiter.mass, central_mass.mass);
  planets.push_back(orbiter);
  Planet orbiter2;
  double r2 = 370.0;
  orbiter2.radius =1.0;
  orbiter2.position = central_mass.position + glm::dvec2(0.0, r2);
  orbiter2.velocity = glm::dvec2( -5.0, -10.0);
  orbiter2.force = GravitationalForce(central_mass.position - orbiter2.position, orbiter2.mass, central_mass.mass);
  planets.push_back(orbiter2);*/

}
void Universe::ProcessInput(GLFWwindow *window)
{
  if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
      glfwSetWindowShouldClose(window, true);
  if(glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
    mainCamera.zoom *= 1.01;
  if(glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
    mainCamera.zoom /= 1.01;
  mainCamera.zoom = glm::clamp<float>(mainCamera.zoom, 0.1, 100000.0);
  //
  constexpr float speed = 20.0;
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
  mainCamera.center += motion;

}
void Universe::Update(float dt)
{
  /* Uses a velocity verlet integrator to update the state of the planets
  Also simulates collisions through static and dynamic collision detection
  */
  #pragma omp parallel for schedule(dynamic)
  {
    //#pragma omp for schedule(dynamic)
      for (int i = 0; i < planets.size(); i++){
        planets[i].velocity += glm::dmat2(0.5*dt/planets[i].mass) * planets[i].force;
        planets[i].position += glm::dmat2(dt) * planets[i].velocity;
        planets[i].force = glm::dvec2(0.0,0.0);
      }

    //#pragma omp for schedule(dynamic)
      for (int i = 0; i < planets.size(); i++){
        for (int j = i+1; j < planets.size(); j++){
            glm::dvec2 distanceVec = planets[j].position - planets[i].position;
            double distance = glm::length(distanceVec);
            double ctime = CollisionTime(planets[i].position, planets[j].position, planets[i].velocity - planets[j].velocity, planets[i].radius + planets[j].radius, dt);
            if (ctime >= 0.0 && ctime <= 1.0 || distance <= (planets[i].radius + planets[j].radius))
            {
              glm::dvec2 xAxis = glm::normalize(distanceVec);// points from the planet *it to *jt
              if(distance >= abs(planets[i].radius - planets[j].radius))
              {

                auto displacement = [](double d, double r1, double r2){
                  return (2*d*r1-r1*r1-d*d+r2*r2)/(2*d);
                };

                planets[i].position += -displacement(distance, planets[i].radius, planets[j].radius)*xAxis;
                planets[j].position += displacement(distance, planets[j].radius, planets[i].radius)*xAxis;
              }
              else
              {
                glm::dvec2 midpoint;
                if (planets[i].radius > planets[j].radius){
                  midpoint = 0.5*(planets[i].position + xAxis*planets[i].radius + planets[j].position);
                }
                else{
                  midpoint = 0.5*(planets[i].position + planets[j].position - xAxis*planets[j].radius);
                }
                auto displacement = [](glm::dvec2 pos, glm::dvec2 mid, double radius){
                  return radius - glm::length(mid - pos);
                };
                planets[i].position += -dt*displacement(planets[i].position, midpoint, planets[i].radius)*xAxis;
                planets[j].position += dt*displacement(planets[j].position, midpoint, planets[j].radius)*xAxis;
              }
              std::tie(planets[i].velocity, planets[j].velocity) = CollisionResponse(planets[j].position - planets[i].position, planets[i].velocity, planets[j].velocity, planets[i].mass, planets[j].mass, 0.0);
              planets[i].force -= glm::dot(planets[i].force, xAxis)*xAxis;
              planets[j].force -= glm::dot(planets[j].force, xAxis)*xAxis;
            }
            else
            {
              glm::dvec2 gravForce = GravitationalForce(distanceVec, planets[i].mass, planets[j].mass);
              planets[i].force += gravForce;
              planets[j].force += -gravForce;
            }
        }
      }
    //#pragma omp for schedule(dynamic)
      for (int i = 0; i < planets.size(); i++){
        planets[i].velocity += glm::dmat2(0.5*dt/planets[i].mass) * planets[i].force;
      }
  }
}
void Universe::Render()
{
  for (Planet p: planets){
    p.shader.setMat4("view", mainCamera.getTransform());
    p.shader.setMat4("projection", projection);
    p.draw();
  }
}
void Universe::UpdateProjection(float width, float height)
{
  projection = glm::ortho(0.0f, (float)width, (float)height, 0.0f, -1.0f, 1.0f);
  mainCamera.UpdateView(width, height);
}
