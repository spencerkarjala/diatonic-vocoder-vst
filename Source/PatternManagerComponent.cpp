#include "PatternManagerComponent.h"
#include "Constants.h"

PatternManagerComponent::PatternManagerComponent(PluginProcessor& processor)
    : mProcessorRef(processor) {
  this->addAndMakeVisible(cComboBoxRoot);
  cComboBoxRoot.addItemList(Constants::KEY_LABELS, 1);
  cComboBoxRoot.setSelectedItemIndex(0);
  cComboBoxRoot.addListener(this);

  this->addAndMakeVisible(cComboBoxScale);
  cComboBoxScale.addItemList(Constants::SCALE_LABELS, 1);
  cComboBoxScale.addListener(this);
}

void PatternManagerComponent::paint(juce::Graphics& g) {
  g.fillAll(juce::Colour(255, 0, 255));
}

void PatternManagerComponent::resized() {
  float padding = 4.f;
  float comboBoxHeight = 20.f;
  float comboBoxWidth = 70.f - padding * 2;

  float flexComboBoxHeight = comboBoxHeight / (float)this->getHeight();
  float flexFillHeight = (2 * comboBoxHeight + 2 * padding) / (float)this->getHeight();
  float flexComboBoxWidth = comboBoxWidth / (float)this->getWidth();
  float flexFillWidth = (comboBoxWidth + padding) / (float)this->getWidth();

  juce::FlexBox verticalRoot, horizontalRoot;
  verticalRoot.flexDirection = juce::FlexBox::Direction::column;
  horizontalRoot.flexDirection = juce::FlexBox::Direction::row;

  juce::FlexItem horizontalPadding(comboBoxWidth, padding);

  verticalRoot.items.addArray({
    horizontalPadding,
    juce::FlexItem(cComboBoxRoot).withFlex(flexComboBoxHeight),
    horizontalPadding,
    juce::FlexItem(cComboBoxScale).withFlex(flexComboBoxHeight),
    horizontalPadding.withFlex(flexFillHeight),
  });

  juce::FlexItem main = juce::FlexItem(verticalRoot).withFlex(flexComboBoxWidth);
  juce::FlexItem paddingLeft(padding, (float)this->getHeight());
  juce::FlexItem paddingRight(padding, (float)this->getHeight());

  horizontalRoot.items.addArray({
    paddingLeft,
    main.withFlex(flexComboBoxWidth),
    paddingRight.withFlex(flexFillWidth),
  });

  horizontalRoot.performLayout(this->getLocalBounds().toFloat());
}

void PatternManagerComponent::comboBoxChanged(juce::ComboBox* updatedComboBox) {
  if (updatedComboBox == &cComboBoxRoot) {
    *mProcessorRef.pKeyBase = updatedComboBox->getSelectedItemIndex();
  }
  else if (updatedComboBox == &cComboBoxScale) {
    unsigned int selectedScaleIndex = updatedComboBox->getSelectedItemIndex();
    std::array<float, 12> newAmplitudes = Constants::SCALE_AMPLITUDES[selectedScaleIndex];
    //mHarmonicAmplitudesComponent.setAmplitudes(newAmplitudes);
    for (int amplitudeIndex = 0; amplitudeIndex < 12; ++amplitudeIndex) {
      *mProcessorRef.pHarmonicAmplitudes[amplitudeIndex] = newAmplitudes[amplitudeIndex];
    }
  }
}
