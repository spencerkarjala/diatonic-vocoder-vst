#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "PatternManagerComponent.h"
#include "GlobalParameterComponent.h"

class ControllerComponent : public juce::Component {
public:
  ControllerComponent(PluginProcessor& processor);
  ~ControllerComponent() = default;

  void paint(juce::Graphics& g) override;
  void resized() override;

private:
  PluginProcessor& mProcessorRef;

  PatternManagerComponent cPatternManager;
  GlobalParameterComponent cGlobalParams;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ControllerComponent)
};

