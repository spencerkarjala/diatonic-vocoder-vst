#pragma once

#include <JuceHeader.h>
#include "../../controllers/PluginProcessor.h"

class RootComponent
  : public juce::Component {
public:
  RootComponent(PluginProcessor& processor);
  ~RootComponent() = default;

  void paint(juce::Graphics& g) override;
  void resized() override;

private:
  PluginProcessor& mProcessorRef;
  reactjuce::ReactApplicationRoot mAppRoot;

  juce::Time mTimeLastUpdated;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RootComponent);
};
