#include "OpenGLProject/App.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <vector>

// GLM headers for matrix transformations
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// --- Constructor / Destructor ---
App::App(int width, int height, const std::string &title) : m_window(nullptr) {
  // --- GLFW Initialization ---
  if (!glfwInit()) {
    throw std::runtime_error("Failed to initialize GLFW");
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  // --- Window Creation ---
  m_window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
  if (!m_window) {
    glfwTerminate();
    throw std::runtime_error("Failed to create GLFW window");
  }
  glfwMakeContextCurrent(m_window);
  glfwSwapInterval(1); // Enable V-Sync
}

App::~App() {
  if (m_window) {
    glfwDestroyWindow(m_window);
  }
  glfwTerminate();
}

// --- Public Methods ---
void App::run() {
  init();
  loop();
  cleanup();
}

// --- Private Methods ---
void App::init() {
  // --- GLEW Initialization ---
  if (glewInit() != GLEW_OK) {
    throw std::runtime_error("Failed to initialize GLEW");
  }

  std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;

  createShaderProgram();
  setupTriangle();

  // Set the background color
  glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
}

void App::loop() {
  while (!glfwWindowShouldClose(m_window)) {
    // Input
    if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
      glfwSetWindowShouldClose(m_window, true);
    }

    // Rendering
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(m_shaderProgram);

    // Create a rotation matrix
    float time = (float)glfwGetTime();
    glm::mat4 transform = glm::mat4(1.0f); // Identity matrix
    transform = glm::rotate(transform, time, glm::vec3(0.0f, 0.0f, 1.0f));

    // Get the location of the 'transform' uniform and set its value
    unsigned int transformLoc =
        glGetUniformLocation(m_shaderProgram, "transform");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

    glBindVertexArray(m_VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    // Swap buffers and poll events
    glfwSwapBuffers(m_window);
    glfwPollEvents();
  }
}

void App::cleanup() {
  glDeleteVertexArrays(1, &m_VAO);
  glDeleteBuffers(1, &m_VBO);
  glDeleteProgram(m_shaderProgram);
}

// --- Helper Functions ---
std::string App::loadShaderSource(const std::string &filepath) {
  std::ifstream file(filepath);
  if (!file.is_open()) {
    throw std::runtime_error("Failed to open shader file: " + filepath);
  }
  std::stringstream buffer;
  buffer << file.rdbuf();
  return buffer.str();
}

void App::createShaderProgram() {
  // Load shader sources
  std::string vertSourceStr = loadShaderSource("assets/shaders/shader.vert");
  std::string fragSourceStr = loadShaderSource("assets/shaders/shader.frag");
  const char *vertSource = vertSourceStr.c_str();
  const char *fragSource = fragSourceStr.c_str();

  // Compile vertex shader
  unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertSource, NULL);
  glCompileShader(vertexShader);
  // (Error checking is omitted for brevity but recommended in production)

  // Compile fragment shader
  unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragSource, NULL);
  glCompileShader(fragmentShader);

  // Link shaders into a program
  m_shaderProgram = glCreateProgram();
  glAttachShader(m_shaderProgram, vertexShader);
  glAttachShader(m_shaderProgram, fragmentShader);
  glLinkProgram(m_shaderProgram);

  // Delete the shaders as they're now linked into our program
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
}

void App::setupTriangle() {
  float vertices[] = {// positions         // colors
                      -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.5f, -0.5f, 0.0f,
                      0.0f,  1.0f,  0.0f, 0.0f, 0.5f, 0.0f, 0.0f, 0.0f,  1.0f};

  glGenVertexArrays(1, &m_VAO);
  glGenBuffers(1, &m_VBO);

  glBindVertexArray(m_VAO);

  glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // Position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  // Color attribute
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
}
