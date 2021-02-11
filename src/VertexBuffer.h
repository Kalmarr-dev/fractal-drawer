#pragma once

#include "glad.h"

class VertexBuffer {
private:
public:
  unsigned int m_RendererID;
  unsigned int m_Count;

  VertexBuffer(const void* data, unsigned int size);
  ~VertexBuffer();

  void Bind() const;
  void Unbind() const;
};
