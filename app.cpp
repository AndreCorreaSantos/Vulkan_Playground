#include "app.hpp"

namespace ve {

void app::run() {
  while (!window.shouldClose()) {
    glfwPollEvents();
  }
}
} 