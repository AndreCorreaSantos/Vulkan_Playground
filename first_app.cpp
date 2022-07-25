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
#define DB_PERLIN_IMPL
#include "db_perlin.hpp"


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
    camera.setPerspectiveProjection(glm::radians(50.f), aspect, 0.1f, 1000.f);

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

  auto currentTime = std::chrono::high_resolution_clock::now();

  float scale = 0.6f;

  for ( int u = 0; u < numpoints; u++){
    float y = u;
    for (int i = 0; i < numpoints; i++){
      float x = i;

      float X = 1.0f;
      float z = 40*db::perlin(x / 64.0f, float(y) /64.0f,0.0f);
      vertices.push_back({{x*scale - 2 ,z,y*scale - 2},{.1f, .1f, z}});
    }
  }
  return vertices;
}
std::vector<uint32_t> generateIndices(int numpoints){
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

// temporary helper function, creates a 1x1x1 mesh centered at offset with an index buffer
std::unique_ptr<LveModel> createmeshModel(LveDevice& device, glm::vec3 offset) {
  LveModel::Builder modelBuilder{};
  int numpoints = 1000;
  modelBuilder.vertices = generateMesh(numpoints);
  // {
  //     // bottom face (red)
  //     {{-.5f, .0f, -.5f}, {.8f, .1f, .1f}},
  //     {{.5f, .0f, .5f}, {.8f, .1f, .1f}},
  //     {{-.5f, .0f, .5f}, {.8f, .1f, .1f}},
  //     {{.5f, .0f, -.5f}, {.8f, .1f, .1f}},
  // };
  for (auto& v : modelBuilder.vertices) {
    v.position += offset;
  }

  modelBuilder.indices = generateIndices(numpoints);
  //{0,  1,  2,  0,  3,  1};

  return std::make_unique<LveModel>(device, modelBuilder);
}

void FirstApp::loadGameObjects() {
  std::shared_ptr<LveModel> lveModel = createmeshModel(lveDevice, {.0f, .0f, .0f});
  auto mesh = LveGameObject::createGameObject();
  mesh.model = lveModel;
  mesh.transform.translation = {.0f, .0f, 2.5f};
  mesh.transform.scale = {.5f, .5f, .5f};
  gameObjects.push_back(std::move(mesh));
}

}  // namespace lve