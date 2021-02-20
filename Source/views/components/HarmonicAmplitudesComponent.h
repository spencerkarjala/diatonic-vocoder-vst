#pragma once

#include <JuceHeader.h>
#include "../drawables/GLDrawable.h"

class HarmonicAmplitudesComponent
    : public juce::Component,
      private juce::OpenGLRenderer {
public:
  HarmonicAmplitudesComponent();
  ~HarmonicAmplitudesComponent();

  void paint(juce::Graphics& g) override;

  void newOpenGLContextCreated() override;
  void renderOpenGL() override;
  void openGLContextClosing() override;

  void mouseMove(const juce::MouseEvent& event) override;
  void mouseDown(const juce::MouseEvent& event) override;
  void mouseDrag(const juce::MouseEvent& event) override;
  void mouseUp(const juce::MouseEvent& event) override;
  void mouseExit(const juce::MouseEvent& event) override;

  class Listener {
  public:
    virtual ~Listener() = default;
    virtual void onAmplitudeChanged(unsigned int index, float value) = 0;
  };

  void registerListener(Listener* newListener);
  void setAmplitudes(std::array<float, 12> newAmplitudes);

private:
  void updateHarmonicAmplitude(unsigned int index, float value);

  std::vector<Listener*> mListeners;

  class HarmonicSliders {
  public:
    HarmonicSliders(juce::OpenGLContext* openGLContext)
        : mCircleIndicators(openGLContext, GL_POINTS) {
      this->init(openGLContext);
    }

    ~HarmonicSliders() = default;

    void draw(GLuint& vbo, GLuint& ibo) {
      mCircleIndicators.draw(vbo, ibo);
    }

    std::vector<float> getPointPositions() {
      std::vector<float> pointPositions;
      std::vector<Vertex> pointVertices = mCircleIndicators.getVertexBuffer();

      for (int point = 0; point < 12; ++point) {
        Vertex pointVertex = pointVertices[point];
        pointPositions.push_back(pointVertex.position[0]);
        pointPositions.push_back(pointVertex.position[1]);
      }
      return pointPositions;
    }

    void updateSliderAmplitude(int slider, float amplitude) {
      Vertex vert = mCircleIndicators.getVertex(slider);
      vert.position[1] = amplitude;
      mCircleIndicators.setVertex(slider, vert);
    };

  private:
    GLDrawable mCircleIndicators;

    void init(juce::OpenGLContext* openGLContext) {
      float step = 2.f / 12.f;
      float xMin = -1.f + step / 2.f;
      float xMax = 1.f - step / 2.f;

      std::vector<Vertex> vertices;
      std::vector<unsigned int> indices;
      unsigned int pointIndex = 0;

      for (float xPos = xMin; xPos < xMax + step; xPos += step) {
        vertices.push_back({
          { xPos, 0.f },
          { 0.f, 0.f, 0.f, 0.f },
        });
        indices.push_back(pointIndex);
        pointIndex++;
      }

      mCircleIndicators.setVertexBuffer(vertices);
      mCircleIndicators.setIndexBuffer(indices);
    }
  };

  void initOpenGLContext();
  void generateGridDrawable();
  void generatePointDrawables();
  void generateCircleDrawables();
  void generateCircleVertices(float radius, float* center, float* color, std::vector<Vertex>& vertices, std::vector<unsigned int>& indices);

  void loadShaderFile(std::string& destination, const std::string filename);

  juce::OpenGLContext mOpenGLContext;
  GLuint mVBO;
  GLuint mIBO;
  std::string mVertexShader;
  std::string mFragmentShader;
  std::unique_ptr<juce::OpenGLShaderProgram> mShaderProgram;

  GLDrawable mBackground;
  GLDrawable mDrawableGrid;
  HarmonicSliders mHarmonicSliders;
  std::vector<GLDrawable> mDrawablePoints;
  std::vector<GLDrawable> mDrawableCircles;
  int mHoveredSlider = -1;

  GLDrawable test = GLDrawable(
    &mOpenGLContext,
    GL_LINES,
    { { { -0.125f, 0.5f },
        { 1.f, 1.f, 0.f, 1.f },
      }, {
        { -0.125f, 0.125f },
        { 0.f, 0.f, 1.f, 1.f },
      }, {
        { 0.f, 0.5f },
        { 0.f, 1.f, 0.f, 1.f },
      }, {
        { 0.f, 0.125f },
        { 1.f, 0.f, 0.f, 1.f },
      }, {
        { 0.125f, 0.5f },
        { 0.f, 1.f, 1.f, 1.f },
      }, {
        { 0.125f, 0.125f },
        { 1.f, 0.f, 1.f, 1.f },
      }, {
        { -0.125f, -0.5f },
        { 1.f, 1.f, 0.f, 1.f },
      }, {
        { -0.125f, -0.125f },
        { 0.f, 0.f, 1.f, 1.f },
      }, {
        { 0.f, -0.5f },
        { 0.f, 1.f, 0.f, 1.f },
      }, {
        { 0.f, -0.125f },
        { 1.f, 0.f, 0.f, 1.f },
      }, {
        { 0.125f, -0.5f },
        { 0.f, 1.f, 1.f, 1.f },
      }, {
        { 0.125f, -0.125f },
        { 1.f, 0.f, 1.f, 1.f },
      }, {
        { 0.f, 0.75f },
        { 1.f, 1.f, 0.f, 1.f },
      }, {
        { 0.f, -0.75f },
        { 0.f, 0.f, 1.f, 1.f },
      }, {
        { -0.0675f, 0.f },
        { 0.f, 1.f, 0.f, 1.f },
      }, {
        { 0.0675f, 0.f },
        { 1.f, 0.f, 0.f, 1.f },
      }, {
      }
    }, {
      0, 1,
      2, 3,
      4, 5,
      6, 7,
      8, 9,
      10, 11,
      0, 12,
      12, 4,
      6, 13,
      13, 10,
      1, 9,
      3, 11,
      7, 14,
      5, 15,
    }
  );
};
