#include "Renderer.h"

void Renderer::Clear() {
  glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const {
  shader.Bind();
  va.Bind();
  ib.Bind();
  glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr);
}

void Renderer::DrawDots(const VertexArray& va, const IndexBuffer& ib, const Shader& shader, const float size) const {
  shader.Bind();
  va.Bind();
  ib.Bind();
  glEnable(GL_PROGRAM_POINT_SIZE);
  glPointSize(size);
  glDrawElements(GL_POINTS, ib.GetCount(), GL_UNSIGNED_INT, nullptr);
}

void Renderer::DrawLines(const VertexArray& va, const IndexBuffer& ib, const Shader& shader, const float size) const {
  shader.Bind();
  va.Bind();
  ib.Bind();
  glLineWidth(size);
  glDrawElements(GL_LINES, ib.GetCount(), GL_UNSIGNED_INT, nullptr);
}

void Renderer::DrawLineStrip(const VertexArray& va, const IndexBuffer& ib, const Shader& shader,
                              const float size, unsigned int count, const GLvoid* indicesOffset, const int& indicesIncrement) const {
  shader.Bind();
  va.Bind();
  ib.Bind();
  glLineWidth(size);
  count = count == 0 ? ib.GetCount() : count;
  glDrawElementsBaseVertex(GL_LINE_STRIP, count, GL_UNSIGNED_INT, indicesOffset, indicesIncrement);
}

// void Renderer::DrawLineSegments(const VertexArray& va, const IndexBuffer& ib, const Shader& shader,
//   const std::vector<int>& LinesEndIndices) const {
//   for (int i = 0; i < LinesEndIndices.size(); i++) {
//     if (i == 0) {
//       if (LinesEndIndices.size() == 1) {
//         DrawLineStrip(va, ib, shader, 15);
//       } else {
//         DrawLineStrip(va, ib, shader, 15, LinesEndIndices[1]);
//       }
//     } else {
//       GLintptr drawingOffset = (LinesEndIndices[i-1] + 1) * sizeof(unsigned int);
//       DrawLineStrip(va, ib, shader, 15, LinesEndIndices[i] - LinesEndIndices[i-1], (GLvoid*)drawingOffset);
//     }
//     DrawDots(va, ib, shader, 7);
//   }
// }

// void Renderer::DrawLineSegments(VertexArray& va, const Shader& shader,
//   const VertexBufferLayout& layout) const {
//   for (int i = 0; i < DrawnLines::up_VertexBuffers.size(); i++) {
//     va.AddBuffer(*(DrawnLines::up_VertexBuffers.at(i)), layout);
//     int verticesCount = DrawnLines::up_VertexBuffers.at(i)->m_Count;
//     unsigned int indices[verticesCount];
//     std::cout << "vertexCount " << DrawnLines::up_VertexBuffers.at(i)->m_Count << " vertices " << verticesCount << '\n';
//     std::cout << "ID: " << DrawnLines::up_VertexBuffers.at(i)->m_RendererID << " i: " << i << '\n';
//     for (size_t j = 0; j < verticesCount; j++) {
//       indices[j] = j;
//     }
//     IndexBuffer _ib(indices, (unsigned int) verticesCount);
//
//     DrawLineStrip(va, _ib, shader, 15, verticesCount);
//
//     DrawDots(va, _ib, shader, 7);
//   }
// }


void Renderer::DrawFractalSkeleton(const FractalSkeleton& fractalSkeleton, VertexArray& va, const Shader& shader, const VertexBufferLayout& layout, const float size) const {
  std::vector<Line> lines;
  lines.push_back(fractalSkeleton.mainLine);
  for (size_t i = 0; i < fractalSkeleton.baseLines.size(); i++) {
    lines.push_back(fractalSkeleton.baseLines[i]);
  }
  for (size_t i = 0; i < fractalSkeleton.directionLines.size(); i++) {
    lines.push_back(fractalSkeleton.directionLines[i]);
  }

  float* vertices = new float[lines.size() * 2 * 2];
  unsigned int* indices = new unsigned int[lines.size() * 2];

  for (int i = 0; i < lines.size(); i++) {
    vertices[i * 4 + 0] = lines[i].x0;
    vertices[i * 4 + 1] = lines[i].y0;
    vertices[i * 4 + 2] = lines[i].x1;
    vertices[i * 4 + 3] = lines[i].y1;
    indices[i * 2 + 0] = i * 2 + 0;
    indices[i * 2 + 1] = i * 2 + 1;
  }

  VertexBuffer vb(vertices, 4 * lines.size() * sizeof(float));
  IndexBuffer ib(indices, (unsigned int)lines.size() * 2);

  va.AddBuffer(vb, layout);

  shader.Bind();
  va.Bind();
  ib.Bind();
  glLineWidth(size);
  glDrawElements(GL_LINES, ib.GetCount(), GL_UNSIGNED_INT, nullptr);
}


void Renderer::DrawFractalColored(const Fractal& fractal, VertexArray& va, IndexBuffer& ib, Shader& shader, const Camera& camera, const float size, VertexBufferLayout& layout) const {
  shader.Bind();
  va.Bind();
  ib.Bind();
  glLineWidth(size);

  int linesDrawn = 0;
  std::cout <<   fractal.linesOnTheLayer.size() << '\n';

  for (size_t i = 1; i <= fractal.linesOnTheLayer.size(); i++) {
    int offset = linesDrawn * 2;
    int count = fractal.linesOnTheLayer[i - 1] * 2;
    GLintptr offset_ptr = offset * sizeof(unsigned int);
    shader.SetUniform4f( "u_color", 1.0f - i / fractal.linesOnTheLayer.size(), (float) i / fractal.linesOnTheLayer.size(), 1.0f, 0.5f );
    shader.SetUniform4f( "u_camera", camera.x, camera.y, camera.w, camera.h);
    glDrawElements(GL_LINES, count, GL_UNSIGNED_INT, (GLvoid*)offset_ptr);
    linesDrawn += fractal.linesOnTheLayer[i - 1];
  }

  // float* vertices = new float[fractal.lastReflectionLinesCount * 2 * 2];
  // unsigned int* indices = new unsigned int[fractal.lastReflectionLinesCount * 2];
  //
  // for (int i = 0; i < fractal.lastReflectionLinesCount; i++) {
  //   vertices[i * 4 + 0] = fractal.lastReflectionLines[i].x0;
  //   vertices[i * 4 + 1] = fractal.lastReflectionLines[i].y0;
  //   vertices[i * 4 + 2] = fractal.lastReflectionLines[i].x1;
  //   vertices[i * 4 + 3] = fractal.lastReflectionLines[i].y1;
  //   indices[i * 2 + 0] = i * 2 + 0;
  //   indices[i * 2 + 1] = i * 2 + 1;
  // }
  //
  // VertexBuffer reflection_vb(vertices, 4 * fractal.lastReflectionLinesCount * sizeof(float));
  // IndexBuffer reflection_ib(indices, (unsigned int)fractal.lastReflectionLinesCount * 2);
  //
  // va.AddBuffer(reflection_vb, layout);
  //
  // std::cout << "reflection_ib count: " << reflection_ib.GetCount() << '\n';
  //
  // shader.SetUniform4f( "u_color", 1.0f, 1.0f, 0.0f, 0.5f );
  // shader.SetUniform4f( "u_camera", camera.x, camera.y, camera.w, camera.h);
  // glDrawElements(GL_LINES, reflection_ib.GetCount(), GL_UNSIGNED_INT, nullptr);
  //
  // delete[] vertices;
  // delete[] indices;
}


void Renderer::DrawFractalRegion(const FractalRegion& fractalRegion, VertexArray& va, IndexBuffer& ib, Shader& shader, const Camera& camera, const float size, VertexBufferLayout& layout) const {
  shader.Bind();
  va.Bind();
  ib.Bind();
  glLineWidth(size);

  shader.SetUniform4f( "u_color", 1.0f, 0.0f, 1.0f, 0.5f );
  shader.SetUniform4f( "u_camera", camera.x, camera.y, camera.w, camera.h);
  float matrix[4] = {1.0f, 0.0f, 0.0f, 1.0f};
  shader.SetUniformMatrix2fv( "u_scaleRotationMatrix", 1, false, matrix);
  shader.SetUniform4f( "u_rotationOrigin", fractalRegion.fractal->mainLine.x0, fractalRegion.fractal->mainLine.y0, 0.0f, 0.0f);
  shader.SetUniform4f( "u_mainLine", fractalRegion.fractal->mainLine.x0, fractalRegion.fractal->mainLine.y0, 0.0f, 0.0f);
  glDrawElements(GL_LINES, ib.GetCount(), GL_UNSIGNED_INT, nullptr);

  double fractalMainLineLengthSquared = fractalRegion.fractal->mainLine.GetLengthSquared();

  for (int i = fractalRegion.expandingFractals.size() - 1; i >= 0; i--) {
    double scalingFactorSquared = fractalRegion.expandingFractals[i].mainLine.GetLengthSquared() / fractalMainLineLengthSquared;
    double acceptableDistanceSquared = (std::max(camera.w, camera.h) * std::max(camera.w, camera.h) + std::max(fractalRegion.fractalWidth, fractalRegion.fractalHeight) * std::max(fractalRegion.fractalWidth, fractalRegion.fractalHeight) * scalingFactorSquared) * 2;
    double distance = (std::abs(camera.x - fractalRegion.expandingFractals[i].mainLine.x0) + std::abs(camera.y - fractalRegion.expandingFractals[i].mainLine.y0)) / 1.4;
    double distanceSquared = distance * distance;
    if ( ! (distanceSquared < acceptableDistanceSquared) ) {
      continue;
    }
    // shader.SetUniform4f( "u_color", 1.0f - fractalRegion.expandingFractals[i].layer / fractalRegion.maxExpandingFractalLayer, (float) fractalRegion.expandingFractals[i].layer / fractalRegion.maxExpandingFractalLayer, 1.0f, 0.5f );
    shader.SetUniform4f( "u_color", std::sin(fractalRegion.expandingFractals[i].layer / 3) / 2 + 0.5f, 1.0f - std::sin(fractalRegion.expandingFractals[i].layer / 4) / 2 - 0.5f, 1.0f, 0.5f );
    // shader.SetUniform4f( "u_color", 0.2f, 0.8f, 1.0f, 0.5f );
    shader.SetUniform4f( "u_camera", camera.x, camera.y, camera.w, camera.h);
    float matrixExpFrac[4] = {static_cast<float>(fractalRegion.expandingFractals[i].srm.scaledCos), static_cast<float>(fractalRegion.expandingFractals[i].srm.scaledSin), - static_cast<float>(fractalRegion.expandingFractals[i].srm.scaledSin), static_cast<float>(fractalRegion.expandingFractals[i].srm.scaledCos)};
    shader.SetUniformMatrix2fv( "u_scaleRotationMatrix", 1, false, matrixExpFrac);
    shader.SetUniform4f( "u_rotationOrigin", fractalRegion.fractal->mainLine.x0, fractalRegion.fractal->mainLine.y0, 0.0f, 0.0f);
    shader.SetUniform4f( "u_mainLine", fractalRegion.expandingFractals[i].mainLine.x0, fractalRegion.expandingFractals[i].mainLine.y0, fractalRegion.expandingFractals[i].mainLine.x1, fractalRegion.expandingFractals[i].mainLine.y1);
    glDrawElements(GL_LINES, ib.GetCount(), GL_UNSIGNED_INT, nullptr);
  }
}
