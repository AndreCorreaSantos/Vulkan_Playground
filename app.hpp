#pragma once

#include "pipeline.hpp"
#include "window.hpp"
#include "lve_device.hpp"
namespace lve {
class FirstApp {
 public:
  static constexpr int WIDTH = 800;
  static constexpr int HEIGHT = 600;

  void run();

 private:
  LveWindow lveWindow{WIDTH, HEIGHT, "Hello Vulkan!"};
  LveDevice lveDevice{lveWindow};
  LvePipeline lvePipeline{lveDevice, "shaders/vert.spv", "shaders/frag.spv",LvePipeline::defaultPipelineConfigInfo(WIDTH,HEIGHT)};
};

}  // namespace lve