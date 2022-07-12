#include "app.hpp"

// std
#include <cstdlib>
#include <iostream>
#include <stdexcept>

int main(){
    ve::app app1{};

  try {
    app1.run();
  } catch (const std::exception &e) {
    std::cerr << e.what() << '\n';
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
} 
