#pragma once
#include <JuceHeader.h>
#include "react_juce.h"

class MeterComponent : public reactjuce::View {
public:
  MeterComponent();
  ~MeterComponent() = default;

  void paint(juce::Graphics& g) override;
  void resized() override;

  void setProperty(const juce::Identifier& name, const juce::var& value) override;

  static void registerView(reactjuce::ReactApplicationRoot& appRoot);

private:
  juce::Path mMeterUnfilled, mMeterFilled;
  juce::Colour mColorUnfilled, mColorFilled;

  const struct ReactProps {
    std::string COLOR_UNFILLED = std::string("colorUnfilled");
    std::string COLOR_FILLED = std::string("colorFilled");
  };
  ReactProps mProps;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MeterComponent)
};
