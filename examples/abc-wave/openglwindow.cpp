#include "openglwindow.hpp"

#include <imgui.h>

#include <cppitertools/itertools.hpp>
#include <glm/gtc/matrix_inverse.hpp>

#include "imfilebrowser.h"

void OpenGLWindow::handleEvent(SDL_Event &ev) {
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
  abcg::glClearColor((197 / 255.0f), (94 / 255.0f), (120 / 255.0f),
                     1);  // Dark Pink

  // Enable depth buffering
  abcg::glEnable(GL_DEPTH_TEST);

  // Create programs
  for (const auto &name : m_shaderNames) {
    const auto path{getAssetsPath() + "shaders/" + name};
    const auto program{createProgramFromFile(path + ".vert", path + ".frag")};
    m_programs.push_back(program);
  }

  // Load model - David
  loadModel(getAssetsPath() + "head-of-david.obj", m_david);

  // Load model - Greek column
  loadModel(getAssetsPath() + "greek-column.obj", m_column);

  // Load model - Palm tree
  loadModel(getAssetsPath() + "palm-tree.obj", m_palm);

  // Load model - Tile
  loadModel(getAssetsPath() + "tile.obj", m_tile);

  m_mappingMode = 1;          // From mesh option
  m_currentProgramIndex = 1;  // Texture shader

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

void OpenGLWindow::loadModel(std::string_view path, Model &m_model) {
  m_model.terminateGL();

  m_model.loadDiffuseTexture(getAssetsPath() + "maps/pattern.jpg");
  m_model.loadNormalTexture(getAssetsPath() + "maps/pattern_normal.jpg");
  m_model.loadObj(path);
  m_model.setupVAO(m_programs.at(m_currentProgramIndex));
}

void OpenGLWindow::paintGL() {
  update();

  // Clear color buffer and depth buffer
  abcg::glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  abcg::glViewport(0, 0, m_viewportWidth, m_viewportHeight);

  // Use currently selected program
  const auto program{m_programs.at(m_currentProgramIndex)};
  abcg::glUseProgram(program);

  // Get location of uniform variables
  const GLint viewMatrixLoc{abcg::glGetUniformLocation(program, "viewMatrix")};
  const GLint projMatrixLoc{abcg::glGetUniformLocation(program, "projMatrix")};
  const GLint modelMatrixLoc{
      abcg::glGetUniformLocation(program, "modelMatrix")};
  const GLint normalMatrixLoc{
      abcg::glGetUniformLocation(program, "normalMatrix")};
  const GLint lightDirLoc{
      abcg::glGetUniformLocation(program, "lightDirWorldSpace")};
  const GLint shininessLoc{abcg::glGetUniformLocation(program, "shininess")};
  const GLint IaLoc{abcg::glGetUniformLocation(program, "Ia")};
  const GLint IdLoc{abcg::glGetUniformLocation(program, "Id")};
  const GLint IsLoc{abcg::glGetUniformLocation(program, "Is")};
  const GLint KaLoc{abcg::glGetUniformLocation(program, "Ka")};
  const GLint KdLoc{abcg::glGetUniformLocation(program, "Kd")};
  const GLint KsLoc{abcg::glGetUniformLocation(program, "Ks")};
  const GLint diffuseTexLoc{abcg::glGetUniformLocation(program, "diffuseTex")};
  const GLint normalTexLoc{abcg::glGetUniformLocation(program, "normalTex")};
  const GLint mappingModeLoc{
      abcg::glGetUniformLocation(program, "mappingMode")};

  // Set uniform variables used by every scene object
  abcg::glUniformMatrix4fv(viewMatrixLoc, 1, GL_FALSE,
                           &m_camera.m_viewMatrix[0][0]);
  abcg::glUniformMatrix4fv(projMatrixLoc, 1, GL_FALSE,
                           &m_camera.m_projMatrix[0][0]);
  abcg::glUniform1i(diffuseTexLoc, 0);
  abcg::glUniform1i(normalTexLoc, 1);

  //   const auto lightDirRotated{m_lightDir};  // TODO remove
  abcg::glUniform4fv(lightDirLoc, 1, &m_lightDir.x);
  abcg::glUniform4fv(IaLoc, 1, &m_Ia.x);
  abcg::glUniform4fv(IdLoc, 1, &m_Id.x);
  abcg::glUniform4fv(IsLoc, 1, &m_Is.x);

  // Set uniform variables of the current object - David
  m_mappingMode = 2;
  abcg::glUniform1i(mappingModeLoc, m_mappingMode);

  m_davidMatrix = glm::mat4{1.0f};
  m_davidMatrix = glm::translate(m_davidMatrix, glm::vec3{-0.02f, 0.0f, 0.25f});
  m_davidMatrix = glm::scale(m_davidMatrix, glm::vec3(0.005f));
  m_davidMatrix =
      glm::rotate(m_davidMatrix, glm::radians(-45.0f), glm::vec3(0, 1, 0));

  abcg::glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, &m_davidMatrix[0][0]);

  const auto davidViewMatrix{glm::mat3(m_viewMatrix * m_davidMatrix)};
  glm::mat3 davidNormalMatrix{glm::inverseTranspose(davidViewMatrix)};
  abcg::glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE,
                           &davidNormalMatrix[0][0]);

  m_Ka = m_david.getKa();
  m_Kd = m_david.getKd();
  m_Ks = m_david.getKs();

  abcg::glUniform1f(shininessLoc, m_david.getShininess());
  abcg::glUniform4fv(KaLoc, 1, &m_Ka.x);
  abcg::glUniform4fv(KdLoc, 1, &m_Kd.x);
  abcg::glUniform4fv(KsLoc, 1, &m_Ks.x);

  m_david.render(m_david.getNumTriangles());

  // Set uniform variables of the current object - Left column
  m_mappingMode = 2;
  abcg::glUniform1i(mappingModeLoc, m_mappingMode);

  m_columnMatrix = glm::mat4{1.0f};
  m_columnMatrix = glm::translate(m_columnMatrix, glm::vec3{-1.0f, 0.0f, 0.0f});
  m_columnMatrix = glm::scale(m_columnMatrix, glm::vec3{0.03f, 0.016f, 0.03f});

  abcg::glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, &m_columnMatrix[0][0]);

  auto columnViewMatrix{glm::mat3(m_viewMatrix * m_columnMatrix)};
  glm::mat3 columnNormalMatrix{glm::inverseTranspose(columnViewMatrix)};
  abcg::glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE,
                           &columnNormalMatrix[0][0]);

  m_Ka = m_column.getKa();
  m_Kd = m_column.getKd();
  m_Ks = m_column.getKs();

  abcg::glUniform1f(shininessLoc, m_column.getShininess());
  abcg::glUniform4fv(KaLoc, 1, &m_Ka.x);
  abcg::glUniform4fv(KdLoc, 1, &m_Kd.x);
  abcg::glUniform4fv(KsLoc, 1, &m_Ks.x);

  m_column.render(m_column.getNumTriangles());

  // Set uniform variables of the current object - Right column
  m_columnMatrix = glm::mat4{1.0f};
  m_columnMatrix = glm::translate(m_columnMatrix, glm::vec3{+1.0f, 0.0f, 0.0f});
  m_columnMatrix = glm::scale(m_columnMatrix, glm::vec3{0.03f, 0.016f, 0.03f});

  abcg::glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, &m_columnMatrix[0][0]);

  columnViewMatrix = glm::mat3(m_viewMatrix * m_columnMatrix);
  columnNormalMatrix = glm::inverseTranspose(columnViewMatrix);
  abcg::glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE,
                           &columnNormalMatrix[0][0]);

  abcg::glUniform1f(shininessLoc, m_column.getShininess());
  abcg::glUniform4fv(KaLoc, 1, &m_Ka.x);
  abcg::glUniform4fv(KdLoc, 1, &m_Kd.x);
  abcg::glUniform4fv(KsLoc, 1, &m_Ks.x);

  m_column.render(m_column.getNumTriangles());

  // Set uniform variables of the current object - Left palm tree
  m_mappingMode = 3;
  abcg::glUniform1i(mappingModeLoc, m_mappingMode);

  m_palmMatrix = glm::mat4{1.0f};
  m_palmMatrix = glm::translate(m_palmMatrix, glm::vec3{+2.0f, 0.0f, -1.0f});
  m_palmMatrix = glm::scale(m_palmMatrix, glm::vec3(0.2f));

  abcg::glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, &m_palmMatrix[0][0]);

  auto palmViewMatrix{glm::mat3(m_viewMatrix * m_palmMatrix)};
  glm::mat3 palmNormalMatrix{glm::inverseTranspose(palmViewMatrix)};
  abcg::glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE,
                           &palmNormalMatrix[0][0]);

  m_Ka = m_palm.getKa();
  m_Kd = m_palm.getKd();
  m_Ks = m_palm.getKs();

  abcg::glUniform1f(shininessLoc, m_palm.getShininess());
  abcg::glUniform4fv(KaLoc, 1, &m_Ka.x);
  abcg::glUniform4fv(KdLoc, 1, &m_Kd.x);
  abcg::glUniform4fv(KsLoc, 1, &m_Ks.x);

  m_palm.render(m_palm.getNumTriangles());

  // Set uniform variables of the current object - Right palm tree
  m_palmMatrix = glm::mat4{1.0f};
  m_palmMatrix = glm::translate(m_palmMatrix, glm::vec3{-2.5f, 0.0f, -2.0f});
  m_palmMatrix = glm::scale(m_palmMatrix, glm::vec3(0.2f));
  m_palmMatrix =
      glm::rotate(m_palmMatrix, glm::radians(-180.0f), glm::vec3(0, 1, 0));

  abcg::glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, &m_palmMatrix[0][0]);

  palmViewMatrix = glm::mat3(m_viewMatrix * m_palmMatrix);
  palmNormalMatrix = glm::inverseTranspose(palmViewMatrix);
  abcg::glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE,
                           &palmNormalMatrix[0][0]);

  abcg::glUniform1f(shininessLoc, m_palm.getShininess());
  abcg::glUniform4fv(KaLoc, 1, &m_Ka.x);
  abcg::glUniform4fv(KdLoc, 1, &m_Kd.x);
  abcg::glUniform4fv(KsLoc, 1, &m_Ks.x);

  m_palm.render(m_palm.getNumTriangles());

  // Set uniform variables of the current object - Second right palm tree
  m_palmMatrix = glm::mat4{1.0f};
  m_palmMatrix = glm::translate(m_palmMatrix, glm::vec3{2.0f, 0.0f, -3.0f});
  m_palmMatrix = glm::scale(m_palmMatrix, glm::vec3(0.2f));

  abcg::glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, &m_palmMatrix[0][0]);

  palmViewMatrix = glm::mat3(m_viewMatrix * m_palmMatrix);
  palmNormalMatrix = glm::inverseTranspose(palmViewMatrix);
  abcg::glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE,
                           &palmNormalMatrix[0][0]);

  abcg::glUniform1f(shininessLoc, m_palm.getShininess());
  abcg::glUniform4fv(KaLoc, 1, &m_Ka.x);
  abcg::glUniform4fv(KdLoc, 1, &m_Kd.x);
  abcg::glUniform4fv(KsLoc, 1, &m_Ks.x);

  m_palm.render(m_palm.getNumTriangles());

  // Set uniform variables of the current object - Second left palm tree
  m_palmMatrix = glm::mat4{1.0f};
  m_palmMatrix = glm::translate(m_palmMatrix, glm::vec3{-2.0f, 0.0f, -4.0f});
  m_palmMatrix = glm::scale(m_palmMatrix, glm::vec3(0.2f));
  m_palmMatrix =
      glm::rotate(m_palmMatrix, glm::radians(-180.0f), glm::vec3(0, 1, 0));

  abcg::glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, &m_palmMatrix[0][0]);

  palmViewMatrix = glm::mat3(m_viewMatrix * m_palmMatrix);
  palmNormalMatrix = glm::inverseTranspose(palmViewMatrix);
  abcg::glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE,
                           &palmNormalMatrix[0][0]);

  abcg::glUniform1f(shininessLoc, m_palm.getShininess());
  abcg::glUniform4fv(KaLoc, 1, &m_Ka.x);
  abcg::glUniform4fv(KdLoc, 1, &m_Kd.x);
  abcg::glUniform4fv(KsLoc, 1, &m_Ks.x);

  m_palm.render(m_palm.getNumTriangles());

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
  for (const auto &program : m_programs) {
    abcg::glDeleteProgram(program);
  }
}

void OpenGLWindow::update() {
  const float deltaTime{static_cast<float>(getDeltaTime())};

  // Update LookAt camera
  m_camera.dolly(m_dollySpeed * deltaTime);
  m_camera.truck(m_truckSpeed * deltaTime);
  m_camera.pan(m_panSpeed * deltaTime);

  // Update tiles
  for (const auto index : iter::range(m_numTiles)) {
    auto &position{m_tilePositions.at(index)};

    // Z coordinate increases by 1 unit per second
    position.z += deltaTime * 1.0f;

    // If this tile is behind the camera, move it to the back
    if (position.z > 0.25 * m_numZTiles / 2 + 1) {
      position.z = -0.25 * m_numZTiles / 2 + 1;
    }
  }
}