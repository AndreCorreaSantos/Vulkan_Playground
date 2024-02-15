#include "first_app.hpp"

#include "keyboard_movement_controller.hpp"
#include "camera.hpp"
#include "simple_render_system.hpp"
#include "objects.hpp"

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

      updateGameObjects(gameObjects);
      simpleRenderSystem.renderGameObjects(commandBuffer, gameObjects, camera);

      lveRenderer.endSwapChainRenderPass(commandBuffer);
      lveRenderer.endFrame();
    }
  }

  vkDeviceWaitIdle(lveDevice.device());
}

void FirstApp::loadGameObjects() {
  std::shared_ptr<LveModel> lvemeshModel = createmeshModel(lveDevice, {.0f, .0f, .0f});// creating plane
  auto mesh = LveGameObject::createGameObject();
  mesh.model = lvemeshModel;
  mesh.transform.translation = {.0f, .0f, 2.5f};
  mesh.transform.scale = {.5f, .5f, .5f};
  gameObjects.push_back(std::move(mesh));

  std::shared_ptr<LveModel> lvesphereModel = createsphereModel(lveDevice, {.0f, .0f, .0f});
  auto sphere = LveGameObject::createGameObject();
  sphere.model = lvesphereModel;
  sphere.transform.translation = {30.0f, -50.0f, 30.0f};
  sphere.transform.velocity = {1.001f,1.001f,1.001f};
  sphere.transform.scale = {.5f, .5f, .5f};
  gameObjects.push_back(std::move(sphere));
}

void FirstApp::updateGameObjects(std::vector<LveGameObject>& gameObjects){
  for(auto& obj : gameObjects){
    obj.transform.translation = obj.transform.velocity*obj.transform.translation;
  }
}

}  // namespace lve