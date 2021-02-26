#include "MeterComponent.h"
#include "../helpers/ResourceHelpers.h"

MeterComponent::MeterComponent() {}

void MeterComponent::paint(juce::Graphics& g) {
  auto strokeType = juce::PathStrokeType(4.f);

  g.setColour(mColorUnfilled);
  g.strokePath(mMeterUnfilled, strokeType, juce::AffineTransform());

  g.setColour(mColorFilled);
  g.strokePath(mMeterFilled, strokeType, juce::AffineTransform());
}

void MeterComponent::resized() {
  this->setBounds(this->getLocalBounds());

  juce::Line<float> meterLine = { 0.f, 0.f, 1.f, 1.f };

  float xCenter = static_cast<float>(this->getWidth()) / 2.f;
  float height= static_cast<float>(this->getHeight());

  mMeterUnfilled = juce::Path();
  mMeterUnfilled.addLineSegment({ xCenter, height, xCenter, 0.f }, 4.f);

  mMeterFilled = juce::Path();
  mMeterFilled.addLineSegment({ xCenter, height, xCenter, height * 0.5f }, 4.f);
}

void MeterComponent::setProperty(const juce::Identifier& name, const juce::var& value) {
  jassert(!name.isNull());
  jassert(!value.isUndefined());

  auto propName = name.toString().toStdString();
  if (name == mProps.COLOR_UNFILLED) {
    jassert(value.isString());
    this->mColorUnfilled = ResourceHelpers::parseColorString(value.toString());
  }
  if (name == mProps.COLOR_FILLED) {
    jassert(value.isString());
    this->mColorFilled = ResourceHelpers::parseColorString(value.toString());
  }
}

void MeterComponent::registerView(reactjuce::ReactApplicationRoot& appRoot) {
  appRoot.registerViewType(
    "MeterComponent",
    []() -> reactjuce::ViewManager::ViewPair {
      auto view = std::make_unique<MeterComponent>();
      auto shadowView = std::make_unique<reactjuce::ShadowView>(view.get());

      return { std::move(view), std::move(shadowView) };
    }
  );
}
