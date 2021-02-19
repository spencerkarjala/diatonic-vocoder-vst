#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

class GlobalParameterComponent
  : public juce::Component,
    private juce::Slider::Listener {
public:
  GlobalParameterComponent(PluginProcessor& processor);
  ~GlobalParameterComponent() = default;

  void paint(juce::Graphics& g) override;
  void resized() override;

  void sliderValueChanged(juce::Slider* updatedSlider);

private:
  PluginProcessor& mProcessorRef;
  juce::Slider cSliderCeiling;
  juce::Slider cSliderFloor;
  juce::Slider cSliderDryWet;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GlobalParameterComponent)
};

