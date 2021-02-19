#pragma once

#include "PluginProcessor.h"
#include "../views/components/HarmonicAmplitudesComponent.h"
#include "../views/components/ControllerComponent.h"

class PluginView
  : public juce::AudioProcessorEditor,
    private HarmonicAmplitudesComponent::Listener {
public:
  explicit PluginView (PluginProcessor&);
  ~PluginView() = default;

  void paint (juce::Graphics&) override;
  void resized() override;

  void onAmplitudeChanged(unsigned int index, float value);
  void onRootChanged(unsigned int newRoot);

private:
  PluginProcessor& mProcessorRef;

  HarmonicAmplitudesComponent cHarmonicAmplitudesComponent;
  ControllerComponent cControllerComponent;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginView)
};
