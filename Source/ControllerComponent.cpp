#include "ControllerComponent.h"
#include "Constants.h"

ControllerComponent::ControllerComponent(PluginProcessor& processor)
    : mProcessorRef(processor),
      cPatternManager(processor),
      cGlobalParams(processor) {
  this->addAndMakeVisible(cPatternManager);
  this->addAndMakeVisible(cGlobalParams);
}

void ControllerComponent::paint(juce::Graphics& g) {
  g.fillAll(juce::Colour(255, 255, 255));
}

void ControllerComponent::resized() {
  int padding = 4;
  int patternManagerWidth = round(0.4f * (float)this->getWidth());
  int globalParamsWidth = this->getWidth() - patternManagerWidth;

  int width = patternManagerWidth - padding * 3 / 2;
  int height = this->getHeight() - 2 * padding;
  int xCenter = padding;
  int yCenter = padding;
  cPatternManager.setBounds(xCenter, yCenter, width, height);

  width = globalParamsWidth - padding * 3 / 2;
  xCenter = patternManagerWidth + padding;
  cGlobalParams.setBounds(xCenter, yCenter, width, height);
}
