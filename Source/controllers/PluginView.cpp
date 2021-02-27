#include "PluginProcessor.h"
#include "PluginView.h"

PluginView::PluginView(PluginProcessor& p)
    : AudioProcessorEditor(&p),
      mProcessorRef(p) {
  cRoot = std::make_unique<RootComponent>(p);
  this->addAndMakeVisible(cRoot.get());

  this->setSize(1000, 500);
  this->setResizable(true, true);

  juce::ComponentBoundsConstrainer* constrainer = new juce::ComponentBoundsConstrainer();
  constrainer->setFixedAspectRatio(2.0);
  constrainer->setSizeLimits(1000, 500, 2400, 1200);
  this->setConstrainer(constrainer);
}

PluginView::~PluginView() {
  delete this->getConstrainer();
}

void PluginView::paint(juce::Graphics& g) {
  g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
}

void PluginView::resized() {
  cRoot->setBounds(this->getLocalBounds());
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
