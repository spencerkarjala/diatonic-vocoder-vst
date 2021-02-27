#pragma once

#include <JuceHeader.h>
#include "../../controllers/PluginProcessor.h"
#include "SVGComponent.h"

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
  SVGComponent cBackgroundSVG;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GlobalParameterComponent)
};

