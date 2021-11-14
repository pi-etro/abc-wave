#include "openglwindow.hpp"

#include <imgui.h>

#include <cppitertools/itertools.hpp>

void OpenGLWindow::initializeGL() {
  abcg::glClearColor((197 / 255.0f), (94 / 255.0f), (120 / 255.0f),
                     1);  // Dark Pink

  // Enable depth buffering
  abcg::glEnable(GL_DEPTH_TEST);

  // Create program
  m_program = createProgramFromFile(getAssetsPath() + "lookat.vert",
                                    getAssetsPath() + "lookat.frag");

  // Load model - David
  m_david.loadObj(getAssetsPath() + "head-of-david.obj");

  m_david.setupVAO(m_program);

  // Load model - Greek column
  m_column.loadObj(getAssetsPath() + "greek-column.obj");

  m_column.setupVAO(m_program);

  // Load model - Palm tree
  m_palm.loadObj(getAssetsPath() + "palm-tree.obj");

  m_palm.setupVAO(m_program);

  // Load model - Tile
  m_tile.loadObj(getAssetsPath() + "tile.obj");

  m_tile.setupVAO(m_program);

  // Setup tiles
  float x = -0.25 * m_numXTiles / 2;
  float z = -0.25 * m_numZTiles / 2;
  int j = 0;
  for (int i = 0; i < m_numTiles; i++) {
    m_tilePositions.at(i) = glm::vec3{x, 0.0f, z};
    j++;
    x += 0.25;
    if (j == m_numXTiles) {
      j = 0;
      z += 0.25;
      x = -0.25 * m_numXTiles / 2;
    }
  }

  resizeGL(getWindowSettings().width, getWindowSettings().height);
}

void OpenGLWindow::paintGL() {
  update();

  // Clear color buffer and depth buffer
  abcg::glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  abcg::glViewport(0, 0, m_viewportWidth, m_viewportHeight);

  abcg::glUseProgram(m_program);

  // Get location of uniform variables (could be precomputed)
  const GLint viewMatrixLoc{glGetUniformLocation(m_program, "viewMatrix")};
  const GLint projMatrixLoc{glGetUniformLocation(m_program, "projMatrix")};
  const GLint modelMatrixLoc{glGetUniformLocation(m_program, "modelMatrix")};
  const GLint colorLoc{glGetUniformLocation(m_program, "color")};

  // Set uniform variables used by every scene object
  abcg::glUniformMatrix4fv(viewMatrixLoc, 1, GL_FALSE,
                           &m_camera.m_viewMatrix[0][0]);
  abcg::glUniformMatrix4fv(projMatrixLoc, 1, GL_FALSE,
                           &m_camera.m_projMatrix[0][0]);

  abcg::glBindVertexArray(0);

  // Set uniform variables of the current object - David
  m_davidMatrix = glm::mat4{1.0f};
  m_davidMatrix = glm::translate(m_davidMatrix, glm::vec3{-0.02f, 0.0f, 0.25f});
  m_davidMatrix = glm::scale(m_davidMatrix, glm::vec3(0.005f));
  m_davidMatrix =
      glm::rotate(m_davidMatrix, glm::radians(-45.0f), glm::vec3(0, 1, 0));

  abcg::glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, &m_davidMatrix[0][0]);
  abcg::glUniform4f(colorLoc, (255 / 255.0f), (255 / 255.0f), (255 / 255.0f),
                    1.0f);  // White

  m_david.render();

  // Set uniform variables of the current object - Left column
  m_columnMatrix = glm::mat4{1.0f};
  m_columnMatrix = glm::translate(m_columnMatrix, glm::vec3{-1.0f, 0.0f, 0.0f});
  m_columnMatrix = glm::scale(m_columnMatrix, glm::vec3{0.03f, 0.016f, 0.03f});

  abcg::glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, &m_columnMatrix[0][0]);
  abcg::glUniform4f(colorLoc, (242 / 255.0f), (240 / 255.0f), (230 / 255.0f),
                    1.0f);  // Marble White

  m_column.render();

  // Set uniform variables of the current object - Right column
  m_columnMatrix = glm::mat4{1.0f};
  m_columnMatrix = glm::translate(m_columnMatrix, glm::vec3{+1.0f, 0.0f, 0.0f});
  m_columnMatrix = glm::scale(m_columnMatrix, glm::vec3{0.03f, 0.016f, 0.03f});

  abcg::glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, &m_columnMatrix[0][0]);
  abcg::glUniform4f(colorLoc, (242 / 255.0f), (240 / 255.0f), (230 / 255.0f),
                    1.0f);  // Marble White

  m_column.render();

  // Set uniform variables of the current object - Left palm tree
  m_palmMatrix = glm::mat4{1.0f};
  m_palmMatrix = glm::translate(m_palmMatrix, glm::vec3{+2.0f, 0.0f, -1.0f});
  m_palmMatrix = glm::scale(m_palmMatrix, glm::vec3(0.2f));

  abcg::glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, &m_palmMatrix[0][0]);
  abcg::glUniform4f(colorLoc, (71 / 255.0f), (103 / 255.0f), (58 / 255.0f),
                    1.0f);  // Palm Green

  m_palm.render();

  // Set uniform variables of the current object - Right palm tree
  m_palmMatrix = glm::mat4{1.0f};
  m_palmMatrix = glm::translate(m_palmMatrix, glm::vec3{-2.5f, 0.0f, -2.0f});
  m_palmMatrix = glm::scale(m_palmMatrix, glm::vec3(0.2f));
  m_palmMatrix =
      glm::rotate(m_palmMatrix, glm::radians(-180.0f), glm::vec3(0, 1, 0));

  abcg::glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, &m_palmMatrix[0][0]);
  abcg::glUniform4f(colorLoc, (71 / 255.0f), (103 / 255.0f), (58 / 255.0f),
                    1.0f);  // Palm Green

  m_palm.render();

  // Set uniform variables of the current object - Second right palm tree
  m_palmMatrix = glm::mat4{1.0f};
  m_palmMatrix = glm::translate(m_palmMatrix, glm::vec3{2.0f, 0.0f, -3.0f});
  m_palmMatrix = glm::scale(m_palmMatrix, glm::vec3(0.2f));

  abcg::glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, &m_palmMatrix[0][0]);
  abcg::glUniform4f(colorLoc, (71 / 255.0f), (103 / 255.0f), (58 / 255.0f),
                    1.0f);  // Palm Green

  m_palm.render();

  // Set uniform variables of the current object - Second left palm tree
  m_palmMatrix = glm::mat4{1.0f};
  m_palmMatrix = glm::translate(m_palmMatrix, glm::vec3{-2.0f, 0.0f, -4.0f});
  m_palmMatrix = glm::scale(m_palmMatrix, glm::vec3(0.2f));
  m_palmMatrix =
      glm::rotate(m_palmMatrix, glm::radians(-180.0f), glm::vec3(0, 1, 0));

  abcg::glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, &m_palmMatrix[0][0]);
  abcg::glUniform4f(colorLoc, (71 / 255.0f), (103 / 255.0f), (58 / 255.0f),
                    1.0f);  // Palm Green

  m_palm.render();

  // Render each tile
  for (const auto index : iter::range(m_numTiles)) {
    const auto &position{m_tilePositions.at(index)};

    // Compute model matrix of the current star
    glm::mat4 m_tileMatrix{1.0f};
    m_tileMatrix = glm::translate(m_tileMatrix, position);

    // Set uniform variable
    abcg::glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, &m_tileMatrix[0][0]);

    if (index % 2 == 0) {
      abcg::glUniform4f(colorLoc, (120 / 255.0f), (248 / 255.0f),
                        (187 / 255.0f), 1.0f);  // Cyan
    } else {
      abcg::glUniform4f(colorLoc, (255 / 255.0f), (122 / 255.0f),
                        (155 / 255.0f), 1.0f);  // Pink
    }

    m_tile.render();
  }

  abcg::glUseProgram(0);
}

void OpenGLWindow::paintUI() { abcg::OpenGLWindow::paintUI(); }

void OpenGLWindow::resizeGL(int width, int height) {
  m_viewportWidth = width;
  m_viewportHeight = height;

  m_camera.computeProjectionMatrix(width, height);
}

void OpenGLWindow::terminateGL() {
  m_david.terminateGL();
  m_column.terminateGL();
  m_palm.terminateGL();
  m_tile.terminateGL();
  abcg::glDeleteProgram(m_program);
}

void OpenGLWindow::update() {
  const float deltaTime{static_cast<float>(getDeltaTime())};

  // Update tiles
  for (const auto index : iter::range(m_numTiles)) {
    auto &position{m_tilePositions.at(index)};

    // Z coordinate increases by 2 units per second
    position.z += deltaTime * 1.0f;

    // If this tile is behind the camera, move it to the back
    if (position.z > 0.25 * m_numZTiles / 2 + 1) {
      position.z = -0.25 * m_numZTiles / 2 + 1;
    }
  }

  m_camera.computeViewMatrix();
}