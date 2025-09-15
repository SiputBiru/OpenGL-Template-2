#ifndef APP_H
#define APP_H

// This define is crucial when linking GLEW statically
// #define GLEW_STATIC
// #include <GL/glew.h>
#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include <string>

class App {
public:
  App(int width = 800, int height = 600,
      const std::string &title = "OpenGL Project");
  ~App();

  // Disable copying
  App(const App &) = delete;
  App &operator=(const App &) = delete;

  void run();

private:
  // Window state
  GLFWwindow *m_window;

  // OpenGL objects
  unsigned int m_shaderProgram;
  unsigned int m_VAO;
  unsigned int m_VBO;

  // Core application logic
  void init();
  void loop();
  void cleanup();

  // Helper functions
  std::string loadShaderSource(const std::string &filepath);
  void createShaderProgram();
  void setupTriangle();
};

#endif // APP_H
