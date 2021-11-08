#include "openglwindow.hpp"

#include <imgui.h>

#include <cppitertools/itertools.hpp>

void OpenGLWindow::handleEvent(SDL_Event& ev) {
  if (ev.type == SDL_KEYDOWN) {
    if (ev.key.keysym.sym == SDLK_UP || ev.key.keysym.sym == SDLK_w)
      m_dollySpeed = 1.0f;
    if (ev.key.keysym.sym == SDLK_DOWN || ev.key.keysym.sym == SDLK_s)
      m_dollySpeed = -1.0f;
    if (ev.key.keysym.sym == SDLK_LEFT || ev.key.keysym.sym == SDLK_a)
      m_panSpeed = -1.0f;
    if (ev.key.keysym.sym == SDLK_RIGHT || ev.key.keysym.sym == SDLK_d)
      m_panSpeed = 1.0f;
    if (ev.key.keysym.sym == SDLK_q) m_truckSpeed = -1.0f;
    if (ev.key.keysym.sym == SDLK_e) m_truckSpeed = 1.0f;
  }
  if (ev.type == SDL_KEYUP) {
    if ((ev.key.keysym.sym == SDLK_UP || ev.key.keysym.sym == SDLK_w) &&
        m_dollySpeed > 0)
      m_dollySpeed = 0.0f;
    if ((ev.key.keysym.sym == SDLK_DOWN || ev.key.keysym.sym == SDLK_s) &&
        m_dollySpeed < 0)
      m_dollySpeed = 0.0f;
    if ((ev.key.keysym.sym == SDLK_LEFT || ev.key.keysym.sym == SDLK_a) &&
        m_panSpeed < 0)
      m_panSpeed = 0.0f;
    if ((ev.key.keysym.sym == SDLK_RIGHT || ev.key.keysym.sym == SDLK_d) &&
        m_panSpeed > 0)
      m_panSpeed = 0.0f;
    if (ev.key.keysym.sym == SDLK_q && m_truckSpeed < 0) m_truckSpeed = 0.0f;
    if (ev.key.keysym.sym == SDLK_e && m_truckSpeed > 0) m_truckSpeed = 0.0f;
  }
}

void OpenGLWindow::initializeGL() {
  abcg::glClearColor((197 / 255.0f), (94 / 255.0f), (120 / 255.0f), 1);

  // Enable depth buffering
  abcg::glEnable(GL_DEPTH_TEST);

  // Create program
  m_program = createProgramFromFile(getAssetsPath() + "lookat.vert",
                                    getAssetsPath() + "lookat.frag");

  m_ground.initializeGL(m_program);

  // Load model - David
  m_david.loadObj(getAssetsPath() + "head-of-david.obj");

  m_david.setupVAO(m_program);

  // Load model - Greek column
  m_column.loadObj(getAssetsPath() + "greek-column.obj");

  m_column.setupVAO(m_program);

  // Load model - Palm tree
  m_palm.loadObj(getAssetsPath() + "palm-tree.obj");

  m_palm.setupVAO(m_program);

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

  abcg::glBindVertexArray(0);

  // Draw ground
  m_ground.paintGL();

  abcg::glUseProgram(0);
}

void OpenGLWindow::paintUI() { abcg::OpenGLWindow::paintUI(); }

void OpenGLWindow::resizeGL(int width, int height) {
  m_viewportWidth = width;
  m_viewportHeight = height;

  m_camera.computeProjectionMatrix(width, height);
}

void OpenGLWindow::terminateGL() {
  m_ground.terminateGL();
  m_david.terminateGL();
  m_column.terminateGL();
  m_palm.terminateGL();
  abcg::glDeleteProgram(m_program);
}

void OpenGLWindow::update() {
  const float deltaTime{static_cast<float>(getDeltaTime())};

  // Update LookAt camera
  m_camera.dolly(m_dollySpeed * deltaTime);
  m_camera.truck(m_truckSpeed * deltaTime);
  m_camera.pan(m_panSpeed * deltaTime);
}