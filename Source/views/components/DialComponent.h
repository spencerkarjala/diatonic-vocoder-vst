#pragma once
#include <JuceHeader.h>
#include "react_juce.h"

class DialComponent : public reactjuce::View {
public:
  DialComponent();
  ~DialComponent() = default;

  void paint(juce::Graphics& g);
  void resized();

  static void registerView(reactjuce::ReactApplicationRoot& appRoot);
  void setProperty(const juce::Identifier& name, const juce::var& value);

private:
  const struct ReactProps {
    std::string VALUE = std::string("value");
    std::string ARC_WIDTH = std::string("arcWidth");
    std::string ARC_LENGTH = std::string("arcLength");
  };
  ReactProps mProps;

  double mValue;
  double mArcWidth;
  double mArcLength;
  juce::Colour mColorDialLight = juce::Colour(0xff1e1e1e);
  juce::Colour mColorDialDark = juce::Colour(0xff131313);

  typedef struct {
    float diameter;
    float radius;
    float xCenter;
    float yCenter;
    float arcWidth;
    float arcLength;
  } Dial;

  void drawDialBase(juce::Graphics& g, Dial d);
  void drawArc(juce::Graphics& g, Dial d, juce::Colour color, float value);
  void drawOffsetCircle(juce::Graphics& g, Dial d, float value, float radius, float relativePos);

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DialComponent)
};
