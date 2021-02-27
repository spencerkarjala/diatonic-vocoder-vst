#include "RootComponent.h"
#include "../helpers/ResourceHelpers.h"
#include "SVGComponent.h"
#include "DialComponent.h"
#include "FloorCeilingSliderComponent.h"
#include "MeterComponent.h"
#include "ParametricComponent.h"

RootComponent::RootComponent(PluginProcessor& processor)
    : mProcessorRef(processor) {
  this->addAndMakeVisible(mAppRoot);
  SVGComponent::registerView(mAppRoot);
  DialComponent::registerView(mAppRoot);
  FloorCeilingSliderComponent::registerView(mAppRoot);
  MeterComponent::registerView(mAppRoot);
  ParametricComponent::registerView(mAppRoot);

  auto jsDirectory = ResourceHelpers::getJSPath();
  juce::File bundle = juce::File(jsDirectory.string());
  mAppRoot.evaluate(bundle);
}

void RootComponent::paint(juce::Graphics& g) {}

void RootComponent::resized() {
  mAppRoot.setBounds(getLocalBounds());
}
