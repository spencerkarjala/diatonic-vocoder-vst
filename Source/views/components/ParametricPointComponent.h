#pragma once
#include <JuceHeader.h>

class ParametricPointComponent : public juce::Component {
public:
  ParametricPointComponent() = default;
  ParametricPointComponent(juce::Point<int> position, int radius, std::function<void(ParametricPointComponent*)> handlePointUpdate);
  ~ParametricPointComponent() = default;

  void paint(juce::Graphics& g) override;
  void resized() override;

  void mouseMove(const juce::MouseEvent& event) override;
  void mouseDrag(const juce::MouseEvent& event) override;
  void mouseEnter(const juce::MouseEvent& event) override;
  void mouseExit(const juce::MouseEvent& event) override;

  float x();
  float y();
  void scalePosition(float x, float y);

private:
  juce::Colour mColor = juce::Colour(juce::Colours::beige);
  juce::Point<float> mPosition = juce::Point<float>(0.f, 0.f);
  int mRadius = 6;
  int mWidth = 12;
  int mHeight = 12;

  std::function<void(ParametricPointComponent*)> mOnPointUpdated;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ParametricPointComponent)
};
