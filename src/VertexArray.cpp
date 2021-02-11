#include "VertexArray.h"
#include <iostream>

VertexArray::VertexArray() {
  glGenVertexArrays(1, &m_RendererID);
}

VertexArray::~VertexArray() {
  glDeleteVertexArrays(1, &m_RendererID);
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout) {
  Bind();
  vb.Bind();
  const auto& elements = layout.GetElements();
  unsigned int offset = 0;
  for (unsigned int i = 0; i < elements.size(); i++) {
    const auto& element = elements[i];
    glEnableVertexAttribArray(i);
    glVertexAttribPointer(i, element.count, element.type, element.normalized ? GL_TRUE : GL_FALSE, layout.GetStride(), (const void*)offset);
    offset += VertexBufferElement::GetSizeOfType(element.type) * element.count;
  }
}


void VertexArray::Bind() const {
  glBindVertexArray(m_RendererID);
}

void VertexArray::Unbind() const {
  glBindVertexArray(0);
}
