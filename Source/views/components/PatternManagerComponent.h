#pragma once

#include <JuceHeader.h>
#include "../../controllers/PluginProcessor.h"

class PatternManagerComponent
  : public juce::Component,
    private juce::ComboBox::Listener {
public:
  PatternManagerComponent(PluginProcessor& processor);
  ~PatternManagerComponent() = default;

  void paint(juce::Graphics& g) override;
  void resized() override;

  void comboBoxChanged(juce::ComboBox* updatedComboBox);

private:
  PluginProcessor& mProcessorRef;

  juce::ComboBox cComboBoxRoot;
  juce::ComboBox cComboBoxScale;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PatternManagerComponent)
};

