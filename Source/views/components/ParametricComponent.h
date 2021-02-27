#pragma once
#include <JuceHeader.h>
#include "react_juce.h"
#include "ParametricPointComponent.h"

class ParametricComponent : public reactjuce::View {
public:
  ParametricComponent();
  ~ParametricComponent() = default;

  void paint(juce::Graphics& g) override;
  void resized() override;

  void mouseDown(const juce::MouseEvent& event) override;
  void mouseDoubleClick(const juce::MouseEvent& event) override;
  void mouseUp(const juce::MouseEvent& event) override;
  void mouseMove(const juce::MouseEvent& event) override;
  void mouseDrag(const juce::MouseEvent& event) override;

  void setProperty(const juce::Identifier& name, const juce::var& value) override;
  static void registerView(reactjuce::ReactApplicationRoot& appRoot);

private:
  juce::Path mBorder, mGrid, mParametricCurve;
  juce::HashMap<juce::String, juce::Colour> mColors;
  juce::Array<std::shared_ptr<ParametricPointComponent>> mPoints;
  int mHighlightedPoint = -1;

  void drawCurves();
  std::shared_ptr<ParametricPointComponent> createNewPoint(juce::Point<int> position);
  void handlePointUpdate(ParametricPointComponent* point);

  const struct ReactProps {
    std::string COLORS = std::string("colors");
  };
  ReactProps mProps;

  const juce::NamedValueSet COLOR_DEFAULTS = {
    { "border", "#333" },
    { "grid", "#333" },
    { "parametricCurve", "fff" },
  };

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ParametricComponent)
};
