#include <fstream>
#include <filesystem>
#include "HarmonicAmplitudesComponent.h"
#include "../../models/Constants.h"
#include "../helpers/ResourceHelpers.h"

HarmonicAmplitudesComponent::HarmonicAmplitudesComponent()
    : mBackground(&mOpenGLContext, GL_TRIANGLES),
      mDrawableGrid(&mOpenGLContext, GL_LINES),
      mHarmonicSliders(&mOpenGLContext) {
  this->setOpaque(true);
  this->initOpenGLContext();

  std::vector<Vertex> vertices = { {
      { -1.f, -1.f },
      { 0.f, 0.f, 0.f, 1.f },
    }, {
      { -1.f, 1.f },
      { 0.f, 0.f, 0.f, 1.f },
    }, {
      { 1.f, -1.f },
      { 0.f, 0.f, 0.f, 1.f },
    }, {
      { 1.f, 1.f },
      { 0.f, 0.f, 0.f, 1.f },
    }, {
  } };
  std::vector<unsigned int> indices = {
    0, 1, 2,
    2, 1, 3,
  };
  mBackground.setVertexBuffer(vertices);
  mBackground.setIndexBuffer(indices);
}

HarmonicAmplitudesComponent::~HarmonicAmplitudesComponent() {
  mOpenGLContext.detach();
}

void HarmonicAmplitudesComponent::initOpenGLContext() {
  mOpenGLContext.setRenderer(this);
  mOpenGLContext.setContinuousRepainting(true);

  juce::OpenGLPixelFormat pixelFormat;
  pixelFormat.multisamplingLevel = 4;
  mOpenGLContext.setPixelFormat(pixelFormat);

  mOpenGLContext.attachTo(*this);
}

void HarmonicAmplitudesComponent::paint(juce::Graphics& g) {}

void HarmonicAmplitudesComponent::loadShaderFile(std::string& destination, const std::string filename) {
  destination.clear();

  auto shaderPath = ResourceHelpers::getShaderPath(filename);

  std::ifstream reader(shaderPath.c_str());
  if (reader) {
    std::stringstream buffer;
    buffer << reader.rdbuf();
    destination = std::string(buffer.str());
  }
}

void HarmonicAmplitudesComponent::newOpenGLContextCreated() {
  mOpenGLContext.extensions.glGenBuffers(1, &mVBO);
  mOpenGLContext.extensions.glGenBuffers(1, &mIBO);

  this->loadShaderFile(mVertexShader, "harmonic-amplitudes.vert");
  this->loadShaderFile(mFragmentShader, "harmonic-amplitudes.frag");

  mShaderProgram.reset(new juce::OpenGLShaderProgram(mOpenGLContext));

  if (!mShaderProgram->addVertexShader(mVertexShader)) {
    jassertfalse;
  }
  if (!mShaderProgram->addFragmentShader(mFragmentShader)) {
    jassertfalse;
  }
  if (!mShaderProgram->link()) {
    jassertfalse;
  }

  mShaderProgram->use();

  this->generateGridDrawable();
}

void HarmonicAmplitudesComponent::renderOpenGL() {
  if (mShaderProgram == nullptr) {
    return;
  }

  std::vector<float> pointPositions = mHarmonicSliders.getPointPositions();

  mShaderProgram->use();
  mShaderProgram->setUniform("windowBounds", (float)this->getWidth(), (float)this->getHeight());
  mShaderProgram->setUniform("pointPositions", &pointPositions[0], pointPositions.size());
  mShaderProgram->setUniform("radius", 5.f);
  mShaderProgram->setUniform("hoveredSlider", mHoveredSlider);

  juce::OpenGLHelpers::clear(juce::Colours::black);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  mBackground.draw(mVBO, mIBO);
  mDrawableGrid.draw(mVBO, mIBO);
  mHarmonicSliders.draw(mVBO, mIBO);
  //test.draw(mVBO, mIBO);
}

void HarmonicAmplitudesComponent::openGLContextClosing() {}

void HarmonicAmplitudesComponent::mouseMove(const juce::MouseEvent& event) {
  juce::Point<int> mousePosition = event.getPosition();
  mHoveredSlider = (int)(12.f * (float)mousePosition.x / this->getWidth());
}

void HarmonicAmplitudesComponent::mouseDown(const juce::MouseEvent& event) {
  juce::Point<int> mousePosition = event.getPosition();
  mHoveredSlider = juce::jlimit(0, 11, (int)(12.f * (float)mousePosition.x / this->getWidth()));
  float newAmplitude = juce::jmap((float)mousePosition.y / this->getHeight(), 1.f, -1.f);
  newAmplitude = juce::jlimit(-1.f, 1.f, newAmplitude);

  this->updateHarmonicAmplitude(mHoveredSlider, newAmplitude);
}

void HarmonicAmplitudesComponent::mouseDrag(const juce::MouseEvent& event) {
  juce::Point<int> mousePosition = event.getPosition();
  float newAmplitude = juce::jmap((float)mousePosition.y / this->getHeight(), 1.f, -1.f);
  newAmplitude = juce::jlimit(-1.f, 1.f, newAmplitude);
  if (event.mods.isRightButtonDown()) {
    mHoveredSlider = juce::jlimit(0, 11, (int)(12.f * (float)mousePosition.x / this->getWidth()));
  }

  this->updateHarmonicAmplitude(mHoveredSlider, newAmplitude);
}

void HarmonicAmplitudesComponent::mouseUp(const juce::MouseEvent& event) {
  int xClipped = juce::jlimit(0, this->getWidth() - 1, event.x);
  int yClipped = juce::jlimit(0, this->getHeight() - 1, event.y);
  if (xClipped != event.x || yClipped != event.y) {
    mHoveredSlider = -1;
  }
}

void HarmonicAmplitudesComponent::mouseExit(const juce::MouseEvent& event) {
  if (!event.mods.isLeftButtonDown() && !event.mods.isRightButtonDown()) {
    mHoveredSlider = -1;
  }
}

void HarmonicAmplitudesComponent::registerListener(Listener* newListener) {
  mListeners.push_back(newListener);
}

void HarmonicAmplitudesComponent::setAmplitudes(std::array<float, 12> newAmplitudes) {
  for (float index = 0; index < 12; ++index) {
    float newAmplitude = newAmplitudes[index] * 2.f - 1.f;
    this->updateHarmonicAmplitude(index, newAmplitude);
  }
}

void HarmonicAmplitudesComponent::updateHarmonicAmplitude(unsigned int index, float value) {
  if (index < 0 || index >= 12) {
    return;
  }

  mHarmonicSliders.updateSliderAmplitude(index, value);
  for (int listener = 0; listener < mListeners.size(); ++listener) {
    mListeners[listener]->onAmplitudeChanged(index, value);
  }
}

void HarmonicAmplitudesComponent::generateGridDrawable() {
  unsigned int numStepsX = 12;
  unsigned int numStepsY = 1;

  float xMin = -1.f;
  float xMax = 1.f;
  float xStep = (xMax - xMin) / (float)numStepsX;

  float yMin = -1.f;
  float yMax = 1.f;
  float yStep = (yMax - yMin) / (float)numStepsY;

  std::vector<Vertex> vertices;
  float color[4] = { 1.f, 1.f, 1.f, 0.1f };

  for (float y = yMin; y < yMax + yStep; y += yStep) {
    for (float x = xMin; x < xMax + xStep; x += xStep) {
      vertices.push_back({
        { x, y },
        { color[0], color[1], color[2], color[3] },
      });
    }
  }

  std::vector<unsigned int> indices = {
    0, 12,
    13, 25,
    0, 13,
    1, 14,
    2, 15,
    3, 16,
    4, 17,
    5, 18,
    6, 19,
    7, 20,
    8, 21,
    9, 22,
    10, 23,
    11, 24,
    12, 25,
  };

  mDrawableGrid.setVertexBuffer(vertices);
  mDrawableGrid.setIndexBuffer(indices);
}

void HarmonicAmplitudesComponent::generatePointDrawables() {
  float step = 2.f / 12.f;
  float xMin = -1.f + step / 2.f;
  float xMax = 1.f - step / 2.f;

  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;
  unsigned int pointIndex = 0;
  
  for (float xPos = xMin; xPos < xMax + step; xPos += step) {
    vertices.push_back({
      { xPos, 0.f },
      { 1.f, 1.f, 1.f, 1.f },
    });
    indices.push_back(pointIndex);
    pointIndex++;
  }
}

void HarmonicAmplitudesComponent::generateCircleDrawables() {}

void HarmonicAmplitudesComponent::generateCircleVertices(float radius, float* center, float* color, std::vector<Vertex>& vertices, std::vector<unsigned int>& indices) {}
