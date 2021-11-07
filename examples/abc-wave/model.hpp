#ifndef MODEL_HPP_
#define MODEL_HPP_

#include <vector>

#include "abcg.hpp"

struct Vertex {
  glm::vec3 position{};

  bool operator==(const Vertex& other) const noexcept {
    return position == other.position;
  }
};

class Model {
 public:
  void loadObj(std::string_view path, bool standardize = false);
  void render(int numTriangles = -1) const;
  void setupVAO(GLuint program);
  void terminateGL();

  [[nodiscard]] int getNumTriangles() const {
    return static_cast<int>(m_indices.size()) / 3;
  }

 private:
  GLuint m_VAO{};
  GLuint m_VBO{};
  GLuint m_EBO{};

  std::vector<Vertex> m_vertices;
  std::vector<GLuint> m_indices;

  void createBuffers();
  void standardize();
};

#endif