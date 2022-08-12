#include "first_app.hpp"

#include "keyboard_movement_controller.hpp"
#include "lve_camera.hpp"
#include "simple_render_system.hpp"

// libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

// std
#include <array>
#include <cassert>
#include <chrono>
#include <stdexcept>
#include <iostream>
#define DB_PERLIN_IMPL
#include "db_perlin.hpp"


namespace lve {

FirstApp::FirstApp() { loadGameObjects(); }

FirstApp::~FirstApp() {}

void FirstApp::run() {
  float time = 0;
  SimpleRenderSystem simpleRenderSystem{lveDevice, lveRenderer.getSwapChainRenderPass()};
  LveCamera camera{};

  auto viewerObject = LveGameObject::createGameObject();
  KeyboardMovementController cameraController{};

  auto currentTime = std::chrono::high_resolution_clock::now();
  while (!lveWindow.shouldClose()) {
    glfwPollEvents();

    auto newTime = std::chrono::high_resolution_clock::now();
    float frameTime =
        std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
    currentTime = newTime;
    time += frameTime;
    cameraController.moveInPlaneXZ(lveWindow.getGLFWwindow(), frameTime, viewerObject);
    camera.setViewYXZ(viewerObject.transform.translation, viewerObject.transform.rotation);

    float aspect = lveRenderer.getAspectRatio();
    camera.setPerspectiveProjection(glm::radians(50.f), aspect, 0.1f, 1000.f);

    if (auto commandBuffer = lveRenderer.beginFrame()) {
      lveRenderer.beginSwapChainRenderPass(commandBuffer);

      updateGameObjects(meshObjects,time);
      simpleRenderSystem.renderGameObjects(commandBuffer, gameObjects,meshObjects,camera);

      lveRenderer.endSwapChainRenderPass(commandBuffer);
      lveRenderer.endFrame();
    }
  }

  vkDeviceWaitIdle(lveDevice.device());
}

std::vector<LveModel::Vertex> generateMeshVertices(int numpoints,float scale){
  std::vector<LveModel::Vertex> vertices;


  for ( int u = 0; u < numpoints; u++){
    float y = u;
    for (int i = 0; i < numpoints; i++){
      float x = i;

      float z = 0;
      glm::vec3 color = {0.43,0.75,0.89};
      vertices.push_back({{(x-numpoints/2)*scale,(y-numpoints/2)*scale,z},color});
    }
  }
  return vertices;
}

std::vector<uint32_t> generateMeshIndices(int numpoints){
  std::vector<uint32_t> indices;
  for (int u =0;u<numpoints-1;u++){
    for (int i =0;i<numpoints-1;i++){
        int c = i*numpoints + u;
        int r = (i+1)*numpoints + u;
        indices.push_back(c);
        indices.push_back(c+1);
        indices.push_back(r);
        indices.push_back(r);
        indices.push_back(r+1);
        indices.push_back(c+1);
    }
  }
  return indices;
}


std::vector<LveModel::Vertex> generateSphereVertices(float radius,float PI,int sectorCount, int stackCount){
std::vector<LveModel::Vertex> vertices;
float x, y, z, xy;                              // vertex position
float nx, ny, nz, lengthInv = 1.0f / radius;    // vertex normal
float s, t;                                     // vertex texCoord

float sectorStep = 2 * PI / sectorCount;
float stackStep = PI / stackCount;
float sectorAngle, stackAngle;

for(int i = 0; i <= stackCount; ++i)
{
    stackAngle = PI / 2 - i * stackStep;        // starting from pi/2 to -pi/2
    xy = radius * cosf(stackAngle);             // r * cos(u)
    z = radius * sinf(stackAngle);              // r * sin(u)

    // add (sectorCount+1) vertices per stack
    // the first and last vertices have same position and normal, but different tex coords
    for(int j = 0; j <= sectorCount; ++j)
    {
        sectorAngle = j * sectorStep;           // starting from 0 to 2pi

        // vertex position (x, y, z)
        x = xy * cosf(sectorAngle);             // r * cos(u) * cos(v)
        y = xy * sinf(sectorAngle);             // r * cos(u) * sin(v)
        glm::vec3 color = {0.48,0.68,0.75};
        vertices.push_back({{x,y,z},color,{x*lengthInv,y*lengthInv,z*lengthInv}});
    }
}
  return vertices;
}

std::vector<uint32_t> generateMeshIndices(int sectorCount, int stackCount){
std::vector<uint32_t> indices;
int k1, k2;
for(int i = 0; i < stackCount; ++i)
{
    k1 = i * (sectorCount + 1);     // beginning of current stack
    k2 = k1 + sectorCount + 1;      // beginning of next stack

    for(int j = 0; j < sectorCount; ++j, ++k1, ++k2)
    {
        // 2 triangles per sector excluding first and last stacks
        // k1 => k2 => k1+1
        if(i != 0)
        {
            indices.push_back(k1);
            indices.push_back(k2);
            indices.push_back(k1 + 1);
        }
        // k1+1 => k2 => k2+1
        if(i != (stackCount-1))
        {
            indices.push_back(k1 + 1);
            indices.push_back(k2);
            indices.push_back(k2 + 1);
        }
    }
}
return indices;
}



// temporary helper function, creates a 1x1x1 mesh centered at offset with an index buffer
std::unique_ptr<LveModel> createmeshModel(LveDevice& device, glm::vec3 offset, int numpoints, float scale) {
  LveModel::Builder modelBuilder{};
  modelBuilder.vertices = generateMeshVertices(numpoints,scale);
  for (auto& v : modelBuilder.vertices) {
    v.position += offset;
  }
  modelBuilder.indices = generateMeshIndices(numpoints);
  return std::make_unique<LveModel>(device, modelBuilder);
}

std::unique_ptr<LveModel> createsphereModel(LveDevice& device, glm::vec3 offset){
  int sectorCount = 500;
  int stackCount = 500;
  float pi = 3.14159f;
  float radius = 30.0f;
  LveModel::Builder modelBuilder{};
  modelBuilder.vertices = generateSphereVertices(radius,pi,sectorCount,stackCount);
  for (auto& v : modelBuilder.vertices) {
    v.position += offset;
  }
  modelBuilder.indices = generateMeshIndices(sectorCount,stackCount);
  return std::make_unique<LveModel>(device, modelBuilder);
}

void FirstApp::updateGameObjects(std::vector<LveGameObject>& gameObjects, float time){
  for(auto& obj : gameObjects){
    obj.time = time;
    obj.transform.translation.x = obj.transform.velocity.x*obj.transform.translation.x;
    obj.transform.translation.y = obj.transform.velocity.y*obj.transform.translation.y;
    obj.transform.translation.z = obj.transform.velocity.z*obj.transform.translation.z;
  }
}

void FirstApp::loadGameObjects() {
  // int numpoints =  4; //translation not working
  // float np = static_cast<float>(numpoints);
  // float scale = 0.01f;
  // std::shared_ptr<LveModel> lvemeshModel = createmeshModel(lveDevice, {.0f, .0f, .0f}, numpoints,scale);
  // auto mesh = LveGameObject::createGameObject();
  // mesh.model = lvemeshModel;
  // mesh.transform.translation = {-np*scale, -np*scale, .0f};
  // mesh.transform.scale = {1.0f, 1.0f, 1.0f};
  // meshObjects.push_back(std::move(mesh));

  std::shared_ptr<LveModel> lvesphereModel = createsphereModel(lveDevice, {.0f, .0f, .0f});
  auto sphere = LveGameObject::createGameObject();
  sphere.model = lvesphereModel;
  sphere.transform.translation = {3.0f, 0.0f, 0.0f};
  sphere.transform.scale = {1.0f, 1.0f, 1.0f};
  meshObjects.push_back(std::move(sphere));

}

}  // namespace lve