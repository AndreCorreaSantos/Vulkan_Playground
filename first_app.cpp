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

namespace lve {

FirstApp::FirstApp() { loadGameObjects(); }

FirstApp::~FirstApp() {}

void FirstApp::run() {
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

    cameraController.moveInPlaneXZ(lveWindow.getGLFWwindow(), frameTime, viewerObject);
    camera.setViewYXZ(viewerObject.transform.translation, viewerObject.transform.rotation);

    float aspect = lveRenderer.getAspectRatio();
    camera.setPerspectiveProjection(glm::radians(50.f), aspect, 0.1f, 10.f);

    if (auto commandBuffer = lveRenderer.beginFrame()) {
      lveRenderer.beginSwapChainRenderPass(commandBuffer);

      simpleRenderSystem.renderGameObjects(commandBuffer, gameObjects, camera);

      lveRenderer.endSwapChainRenderPass(commandBuffer);
      lveRenderer.endFrame();
    }
  }

  vkDeviceWaitIdle(lveDevice.device());
}

std::vector<LveModel::Vertex> generateMesh(int numpoints){
  std::vector<LveModel::Vertex> vertices;
  float scale = 0.1f;
  float z = 0.0f;
  for ( int u = 0; u <= numpoints; u++){
    float x = u;
    for (int i = 0; i <= numpoints; i++){
      float y = i;
      vertices.push_back({{x*scale - 2 ,z,y*scale - 2},{.9f, .9f, .9f}});
    }
  }
  return vertices;
}
std::vector<uint16_t> generateIndices(int numpoints){
  std::vector<uint16_t> indices;
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

std::unique_ptr<LveModel> createModel(LveDevice& device, glm::vec3 offset,int numpoints) {
  std::vector<LveModel::Vertex> vertices = generateMesh(numpoints);


  for (auto& v : vertices) {
    v.position += offset;
  }
  std::vector<uint16_t> indices =  generateIndices(numpoints);
  return std::make_unique<LveModel>(device, vertices,indices);
}

void FirstApp::loadGameObjects() {
    int numpoints = 5;
  std::shared_ptr<LveModel> lveModel = createModel(lveDevice, {.0f, .0f, .0f},numpoints);
  auto object = LveGameObject::createGameObject();
  object.model = lveModel;
  object.transform.translation = {.0f, .0f, 2.5f};
  object.transform.scale = {.5f, .5f, .5f};
  gameObjects.push_back(std::move(object));
}

}  // namespace lve