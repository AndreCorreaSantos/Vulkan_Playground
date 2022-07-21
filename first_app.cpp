#include "first_app.hpp"

#include "simple_render_system.hpp"

// libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

// std
#include <array>
#include <cassert>
#include <stdexcept>

namespace lve {

FirstApp::FirstApp() { loadGameObjects(); }

FirstApp::~FirstApp() {}

void FirstApp::run() {
  SimpleRenderSystem simpleRenderSystem{lveDevice, lveRenderer.getSwapChainRenderPass()};

  while (!lveWindow.shouldClose()) {
    glfwPollEvents();

    if (auto commandBuffer = lveRenderer.beginFrame()) {
      lveRenderer.beginSwapChainRenderPass(commandBuffer);
      simpleRenderSystem.renderGameObjects(commandBuffer, gameObjects);
      lveRenderer.endSwapChainRenderPass(commandBuffer);
      lveRenderer.endFrame();
    }
  }

  vkDeviceWaitIdle(lveDevice.device());
}

void FirstApp::loadGameObjects() {
  std::vector<LveModel::Vertex> vertices{
      {{-1.0f, -1.0f}, {1.0f, 0.0f, 0.0f}},
      {{1.0f, -1.0f}, {0.0f, 1.0f, 0.0f}},
      {{1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}},
      {{1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}},
      {{-1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}},
      {{-1.0f, -1.0f}, {1.0f, 0.0f, 0.0f}}};



  auto lveModel = std::make_shared<LveModel>(lveDevice, vertices);
  auto triangle = LveGameObject::createGameObject();
  triangle.model = lveModel;
  triangle.color = {.1f, 0.02f, 0.1f};
  triangle.transform2d.translation.x = 0.0f;
  triangle.transform2d.translation.y = -0.0f;
  triangle.transform2d.scale = {0.9f, 0.9f};
  triangle.transform2d.rotation =0.0f;//glm::two_pi<float>();
  gameObjects.push_back(std::move(triangle));

}

}  // namespace lve