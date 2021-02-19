#include "PluginProcessor.h"
#include "PluginView.h"

PluginView::PluginView(PluginProcessor& p)
    : AudioProcessorEditor(&p),
      mProcessorRef(p),
      cControllerComponent(mProcessorRef) {
  this->addAndMakeVisible(cHarmonicAmplitudesComponent);
  cHarmonicAmplitudesComponent.registerListener(this);

  this->addAndMakeVisible(cControllerComponent);

  this->setSize(800, 400);
}

void PluginView::paint(juce::Graphics& g) {
  g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
}

void PluginView::resized() {
  juce::FlexBox verticalRoot, horizontalRoot;
  verticalRoot.flexDirection = juce::FlexBox::Direction::column;
  horizontalRoot.flexDirection = juce::FlexBox::Direction::row;

  juce::FlexItem paddingTop((float)this->getWidth() - 8.f, 4.f);
  juce::FlexItem paddingBottom((float)this->getWidth() - 8.f, 4.f);

  verticalRoot.items.add(paddingTop);
  verticalRoot.items.add(juce::FlexItem(cHarmonicAmplitudesComponent).withFlex(3));
  verticalRoot.items.add(juce::FlexItem(cControllerComponent).withFlex(2));
  verticalRoot.items.add(paddingBottom);

  juce::FlexItem main(verticalRoot);
  juce::FlexItem paddingLeft(4.f, (float)this->getHeight());
  juce::FlexItem paddingRight(4.f, (float)this->getHeight());

  horizontalRoot.items.addArray({ paddingLeft, main, paddingRight });

  horizontalRoot.performLayout(this->getLocalBounds().toFloat());
}

void PluginView::onAmplitudeChanged(unsigned int index, float value) {
  *mProcessorRef.pHarmonicAmplitudes[index] = value;
}

void PluginView::onRootChanged(unsigned int newRoot) {
  *mProcessorRef.pKeyBase = newRoot;
}

unsigned roundDownToMultiple(unsigned num, unsigned multiple) {
  unsigned remainder = num % multiple;
  return num - remainder;
}
