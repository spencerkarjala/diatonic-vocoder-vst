#pragma once
#include <JuceHeader.h>

class SVGComponent : public juce::Component {
public:
  SVGComponent(const char* path);
  ~SVGComponent() = default;

  void paint(juce::Graphics& g);
  void resized();

private:
  std::unique_ptr<juce::Drawable> mDrawable;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SVGComponent)
};
