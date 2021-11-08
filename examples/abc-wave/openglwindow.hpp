#ifndef OPENGLWINDOW_HPP_
#define OPENGLWINDOW_HPP_

#include <vector>

#include "abcg.hpp"
#include "camera.hpp"
#include "model.hpp"

class OpenGLWindow : public abcg::OpenGLWindow {
 protected:
  void handleEvent(SDL_Event& ev) override;  // TODO for activity 3
  void initializeGL() override;
  void paintGL() override;
  void paintUI() override;
  void resizeGL(int width, int height) override;
  void terminateGL() override;

 private:
  static const int m_numXTiles{49};
  static const int m_numZTiles{50};
  static const int m_numTiles{m_numXTiles * m_numZTiles};

  GLuint m_program{};

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
  glm::mat4 m_projMatrix{1.0f};

  void update();
};

#endif