#include "OpenGLProject/App.h"
#include <iostream>

int main() {
  try {
    App app;
    app.run();
  } catch (const std::exception &e) {
    std::cerr << "FATAL ERROR: " << e.what() << std::endl;
    return 1;
  }

  return 0;
}
