#include "GLDrawable.h"

GLDrawable::GLDrawable()
  : mOpenGLContext(juce::OpenGLContext()),
    mType(0) {}

GLDrawable::GLDrawable(juce::OpenGLContext* openGLContext, int type)
  : mOpenGLContext(*openGLContext),
    mType(type) {}

GLDrawable::GLDrawable(juce::OpenGLContext* openGLContext, int type, std::vector<Vertex> vertexBuffer, std::vector<unsigned int> indexBuffer)
  : mOpenGLContext(*openGLContext),
    mType(type),
    mVertexBuffer(vertexBuffer),
    mIndexBuffer(indexBuffer) {}

void GLDrawable::draw(GLuint& vbo, GLuint& ibo) {
  if (mOpenGLContext.isAttached()) {
    // bind and fill the vertex buffer
    mOpenGLContext.extensions.glBindBuffer(juce::GL_ARRAY_BUFFER, vbo);
    mOpenGLContext.extensions.glBufferData(juce::GL_ARRAY_BUFFER, sizeof(Vertex) * mVertexBuffer.size(), &mVertexBuffer[0], juce::GL_STATIC_DRAW);

    // bind and fill the index buffer
    mOpenGLContext.extensions.glBindBuffer(juce::GL_ELEMENT_ARRAY_BUFFER, ibo);
    mOpenGLContext.extensions.glBufferData(juce::GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * mIndexBuffer.size(), &mIndexBuffer[0], juce::GL_STATIC_DRAW);

    // set the attribute pointer for the vertices (x,y) at the first two memory locations in the Vertex struct
    mOpenGLContext.extensions.glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), VERTEX_LOCATION);
    mOpenGLContext.extensions.glEnableVertexAttribArray(0);

    // set the color pointer for the data (r,g,b,a) at the last four memory locations in the Vertex struct
    mOpenGLContext.extensions.glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), COLOR_LOCATION);
    mOpenGLContext.extensions.glEnableVertexAttribArray(1);

    // draw, then release the vertex pointers for next object to draw
    glDrawElements(mType, mIndexBuffer.size(), GL_UNSIGNED_INT, nullptr);
    mOpenGLContext.extensions.glDisableVertexAttribArray(0);
    mOpenGLContext.extensions.glDisableVertexAttribArray(1);
  }
}

void GLDrawable::setVertexBuffer(std::vector<Vertex> data) {
  mVertexBuffer = data;
}

void GLDrawable::setIndexBuffer(std::vector<unsigned int> data) {
  mIndexBuffer = data;
}

std::vector<Vertex> GLDrawable::getVertexBuffer() {
  return mVertexBuffer;
}

Vertex GLDrawable::getVertex(unsigned vertexIndex) {
  return mVertexBuffer[vertexIndex];
}

void GLDrawable::setVertex(unsigned vertexIndex, Vertex newVertex) {
  mVertexBuffer[vertexIndex] = newVertex;
}
