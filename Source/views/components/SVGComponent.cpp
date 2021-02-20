#include "SVGComponent.h"
#include "../helpers/ResourceHelpers.h"

SVGComponent::SVGComponent(const char* path) {
  juce::File svgFile(ResourceHelpers::getSVGPath(path).string());
  mDrawable = juce::Drawable::createFromImageFile(svgFile);

  this->addAndMakeVisible(mDrawable.get());
  mDrawable->setBounds(this->getLocalBounds());
}

void SVGComponent::paint(juce::Graphics& g) {}

void SVGComponent::resized() {
  auto drawable = dynamic_cast<juce::DrawableComposite*>(mDrawable.get());
  auto newBounds = (juce::Parallelogram<float>)this->getLocalBounds().toFloat();

  drawable->setBounds(this->getLocalBounds());
  drawable->setBoundingBox(newBounds);
}
