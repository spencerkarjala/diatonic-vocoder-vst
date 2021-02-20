#include "ControllerComponent.h"
#include "../../models/Constants.h"

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
  float padding = 4.f;
  float componentWidth = (float)this->getWidth();
  float componentHeight = (float)this->getHeight();

  float flexRatioPatternManager = 0.4f;
  float flexRatioGlobalParams = 1.f - flexRatioPatternManager;

  float flexPaddingWidth = padding / componentWidth;
  float flexPaddingHeight = padding / componentHeight;
  float flexPatternManagerWidth = (1.f - 3 * flexPaddingWidth) * flexRatioPatternManager;
  float flexGlobalParamsWidth = (1.f - 3 * flexPaddingWidth) * flexRatioGlobalParams;
  float flexContentHeight = 1.f - 2 * flexPaddingHeight;

  juce::FlexBox verticalRoot, horizontalRoot;
  verticalRoot.flexDirection = juce::FlexBox::Direction::column;
  horizontalRoot.flexDirection = juce::FlexBox::Direction::row;

  juce::FlexItem horizontalPadding(componentWidth, padding);
  juce::FlexItem verticalPadding(padding, componentHeight - 2.f * padding);
  juce::FlexItem patternManager(cPatternManager);
  juce::FlexItem globalParameters(cGlobalParams);

  horizontalRoot.items.addArray({
    verticalPadding.withFlex(flexPaddingWidth / 2),
    patternManager.withFlex(flexPatternManagerWidth),
    verticalPadding.withFlex(flexPaddingWidth / 2),
    globalParameters.withFlex(flexGlobalParamsWidth),
    verticalPadding.withFlex(flexPaddingWidth / 2),
  });

  juce::FlexItem main(horizontalRoot);

  verticalRoot.items.addArray({
    horizontalPadding.withFlex(flexPaddingHeight / 2),
    main.withFlex(flexContentHeight),
    horizontalPadding.withFlex(flexPaddingHeight / 2),
  });

  verticalRoot.performLayout(this->getLocalBounds().toFloat());
}
