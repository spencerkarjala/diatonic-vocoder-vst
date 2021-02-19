#pragma once
#include <JuceHeader.h>
#include "GeometryHelpers.h"

class GLDrawable {
public:
  GLDrawable();
  GLDrawable(juce::OpenGLContext* openGLContext, int type);
  GLDrawable(juce::OpenGLContext* openGLContext, int type, std::vector<Vertex> vertexBuffer, std::vector<unsigned int> indexBuffer);
  ~GLDrawable() = default;

  void draw(GLuint& vbo, GLuint& ibo);
  void setVertexBuffer(std::vector<Vertex> data);
  void setIndexBuffer(std::vector<unsigned int> data);
  std::vector<Vertex> getVertexBuffer();
  Vertex getVertex(unsigned vertexIndex);
  void setVertex(unsigned vertexIndex, Vertex newVertex);

private:
  juce::OpenGLContext& mOpenGLContext;
  std::vector<Vertex> mVertexBuffer;
  std::vector<unsigned int> mIndexBuffer;
  int mType;

  GLvoid* VERTEX_LOCATION = nullptr;
  GLvoid* COLOR_LOCATION = (GLvoid*)(sizeof(float) * 2);
};