#pragma once

#include <react_juce.h>
#include "PluginProcessor.h"
#include "../views/components/RootComponent.h"
#include "../views/components/HarmonicAmplitudesComponent.h"

class PluginView
  : public juce::AudioProcessorEditor,
    private HarmonicAmplitudesComponent::Listener {
public:
  explicit PluginView (PluginProcessor&);
  ~PluginView();

  void paint (juce::Graphics&) override;
  void resized() override;

  void onAmplitudeChanged(unsigned int index, float value);
  void onRootChanged(unsigned int newRoot);

private:
  PluginProcessor& mProcessorRef;
  std::unique_ptr<RootComponent> cRoot;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginView)
};
