#pragma once

#include "lve_device.hpp"
#include "lve_game_object.hpp"
#include "lve_renderer.hpp"
#include "lve_window.hpp"

// std
#include <memory>
#include <vector>

namespace lve {
class FirstApp {
 public:
  static constexpr int WIDTH = 2560;
  static constexpr int HEIGHT = 1440;

  FirstApp();
  ~FirstApp();

  FirstApp(const FirstApp &) = delete;
  FirstApp &operator=(const FirstApp &) = delete;

  void run();

 private:
  void updateGameObjects(std::vector<LveGameObject>& gameObjects,float time);
  void loadGameObjects();

  LveWindow lveWindow{WIDTH, HEIGHT, "L-systems"};
  LveDevice lveDevice{lveWindow};
  LveRenderer lveRenderer{lveWindow, lveDevice};

  std::vector<LveGameObject> gameObjects;
  std::vector<LveGameObject> meshObjects;
};
}  // namespace lve