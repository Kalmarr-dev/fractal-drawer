#pragma once

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"
// #include "DrawnLines.h"
#include "FractalSkeleton.h"
#include "Camera.h"
#include "FractalRegion.h"

#include <vector>
#include <iostream>
#include <memory>

class Renderer {
private:

public:
  void Clear();
  void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
  void DrawDots(const VertexArray& va, const IndexBuffer& ib, const Shader& shader, const float size) const;
  void DrawLines(const VertexArray& va, const IndexBuffer& ib, const Shader& shader, const float size) const;
  void DrawLineStrip(const VertexArray& va, const IndexBuffer& ib, const Shader& shader, const float size, unsigned int count = 0, const GLvoid* indicesOffset = nullptr, const int& indicesIncrement = 0) const;
  // void DrawLineSegments(const VertexArray& va, const IndexBuffer& ib, const Shader& shader, const std::vector<int>& LinesEndIndices) const;
  void DrawLineSegments(VertexArray& va, const Shader& shader, const VertexBufferLayout& layout) const;
  void DrawFractalSkeleton(const FractalSkeleton& fractalSkeleton, VertexArray& va, Shader& shader, const VertexBufferLayout& layout, const float size, const Camera& camera) const;
  void DrawFractalColored(const Fractal& fractal, VertexArray& va, IndexBuffer& ib, Shader& shader, const Camera& camera, const float size, VertexBufferLayout& layout) const;
  void DrawFractalRegion(const FractalRegion& fractalRegion, VertexArray& va, IndexBuffer& ib, Shader& shader, const Camera& camera, const float size, VertexBufferLayout& layout) const;
};
