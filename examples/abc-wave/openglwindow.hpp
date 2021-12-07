#ifndef OPENGLWINDOW_HPP_
#define OPENGLWINDOW_HPP_

#include <vector>

#include "abcg.hpp"
#include "camera.hpp"
#include "model.hpp"

class OpenGLWindow : public abcg::OpenGLWindow {
 protected:
  void handleEvent(SDL_Event& ev) override;
  void initializeGL() override;
  void paintGL() override;
  void paintUI() override;
  void resizeGL(int width, int height) override;
  void terminateGL() override;

 private:
  static const int m_numXTiles{49};
  static const int m_numZTiles{90};
  static const int m_numTiles{m_numXTiles * m_numZTiles};

  int m_viewportWidth{};
  int m_viewportHeight{};

  Camera m_camera;
  float m_dollySpeed{0.0f};
  float m_truckSpeed{0.0f};
  float m_panSpeed{0.0f};

  Model m_david;
  Model m_column;
  Model m_palm;
  Model m_tile;

  glm::mat4 m_davidMatrix{1.0f};
  glm::mat4 m_columnMatrix{1.0f};
  glm::mat4 m_palmMatrix{1.0f};
  glm::mat4 m_tileMatrix{1.0f};

  std::array<glm::vec3, m_numTiles> m_tilePositions;

  glm::mat4 m_viewMatrix{1.0f};

  // Shaders
  std::vector<const char*> m_shaderNames{"texture", "blinnphong", "normal"};
  std::vector<GLuint> m_programs;

  // Mapping mode
  // 0: triplanar; 1: cylindrical; 2: spherical; 3: from mesh
  int m_mappingMode{};

  // Light and material properties
  glm::vec4 m_lightDir{-0.75f, 0.05f, -1.0f, 0.0f};
  glm::vec4 m_Ia{1.0f};
  glm::vec4 m_Id{1.0f};
  glm::vec4 m_Is{1.0f};
  glm::vec4 m_Ka{};
  glm::vec4 m_Kd{};
  glm::vec4 m_Ks{};

  void loadModel(std::string_view path, Model& m_model, int programIndex);
  static bool compareZ(const glm::vec3& left, const glm::vec3& right);
  void update();
};

#endif