#pragma once

#include "PluginProcessor.h"

class PluginView : public juce::AudioProcessorEditor {
public:
  explicit PluginView (PluginProcessor&);
  ~PluginView();

  void paint (juce::Graphics&) override;
  void resized() override;

  void onAmplitudeChanged(unsigned int index, float value);
  void onRootChanged(unsigned int newRoot);

private:
  PluginProcessor& mProcessorRef;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginView)
};
