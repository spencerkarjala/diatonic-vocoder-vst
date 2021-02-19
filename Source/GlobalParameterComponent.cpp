#include "GlobalParameterComponent.h"
#include "Constants.h"

GlobalParameterComponent::GlobalParameterComponent(PluginProcessor& processor)
    : mProcessorRef(processor) {
  this->addAndMakeVisible(cSliderCeiling);
  cSliderCeiling.setSliderStyle(juce::Slider::SliderStyle::Rotary);
  cSliderCeiling.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 25);
  cSliderCeiling.setRange(0.f, 1.f, 0.001f);
  cSliderCeiling.addListener(this);

  this->addAndMakeVisible(cSliderFloor);
  cSliderFloor.setSliderStyle(juce::Slider::SliderStyle::Rotary);
  cSliderFloor.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 25);
  cSliderFloor.setRange(0.f, 1.f, 0.001f);
  cSliderFloor.addListener(this);

  this->addAndMakeVisible(cSliderDryWet);
  cSliderDryWet.setSliderStyle(juce::Slider::SliderStyle::Rotary);
  cSliderDryWet.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 25);
  cSliderDryWet.setRange(0.f, 1.f, 0.001f);
  cSliderDryWet.addListener(this);
}

void GlobalParameterComponent::paint(juce::Graphics& g) {
  g.fillAll(juce::Colour(0, 255, 255));
}

void GlobalParameterComponent::resized() {
  float padding = 4.f;
  float dialHeight = 80.f;
  float dialWidth = 80.f;
  float componentHeight = (float)this->getHeight();
  float componentWidth = (float)this->getWidth();

  float flexDialHeight = dialHeight / componentHeight;
  float flexDialWidth = dialWidth / componentWidth;
  float flexFillHeight = (1.f - flexDialHeight) / 2.f;
  float flexFillWidth = (1.f - 3.f * flexDialWidth) / 2.f;

  juce::FlexBox verticalRoot, horizontalRoot;
  verticalRoot.flexDirection = juce::FlexBox::Direction::column;
  horizontalRoot.flexDirection = juce::FlexBox::Direction::row;

  juce::FlexItem horizontalPadding(componentWidth, padding);
  juce::FlexItem verticalPadding(padding, dialHeight);

  horizontalRoot.items.addArray({
    verticalPadding.withFlex(flexFillHeight),
    juce::FlexItem(cSliderCeiling).withFlex(flexDialHeight),
    juce::FlexItem(cSliderFloor).withFlex(flexDialHeight),
    juce::FlexItem(cSliderDryWet).withFlex(flexDialHeight),
    verticalPadding.withFlex(flexFillHeight),
  });

  juce::FlexItem main(horizontalRoot);

  verticalRoot.items.addArray({
    horizontalPadding.withFlex(flexFillWidth),
    main.withFlex(flexDialHeight),
    horizontalPadding.withFlex(flexFillWidth),
  });

  verticalRoot.performLayout(this->getLocalBounds().toFloat());
}

void GlobalParameterComponent::sliderValueChanged(juce::Slider* updatedSlider) {
  if (updatedSlider == &cSliderCeiling) {
    *mProcessorRef.pCeiling = updatedSlider->getValue();
  }
  else if (updatedSlider == &cSliderFloor) {
    *mProcessorRef.pFloor = updatedSlider->getValue();
  }
  else if (updatedSlider == &cSliderDryWet) {
    *mProcessorRef.pDryWet = updatedSlider->getValue();
  }
}
