#include "DialComponent.h"
#include "../helpers/ResourceHelpers.h"

#define PI juce::MathConstants<float>::pi
#define PI_2 2.f * juce::MathConstants<float>::pi

DialComponent::DialComponent() {}

void DialComponent::paint(juce::Graphics& g) {
  float width = (float)this->getWidth();
  float height = (float)this->getHeight();

  Dial dial = {
    juce::jmin(width, height),
    juce::jmin(width, height) / 2.f,
    width / 2.f,
    height / 2.f,
    (float)mArcWidth,
    (float)mArcLength * PI_2,
  };

  auto colorArcBackground = juce::Colour(0xff2c2c2c);
  auto colorArcFilled = juce::Colour(0xff00b894);

  this->drawDialBase(g, dial);

  this->drawArc(g, dial, colorArcBackground, 1.f);
  this->drawArc(g, dial, colorArcFilled, (float)mValue);

  this->drawOffsetCircle(g, dial, (float)mValue, dial.diameter * 0.04f, 0.7f);
  this->drawOffsetCircle(g, dial, (float)mValue, dial.diameter * 0.02f, 0.35f);
}

void DialComponent::drawDialBase(juce::Graphics& g, Dial d) {
  float xStart = d.arcWidth + d.xCenter - d.radius;
  float yStart = d.arcWidth + d.yCenter - d.radius;

  juce::Rectangle<float> area(xStart, yStart, d.diameter - (2.f * d.arcWidth), d.diameter - (2.f * d.arcWidth));
  juce::ColourGradient dialGradient(mColorDialLight, { 0.f, 0.f }, mColorDialDark, { d.diameter, d.diameter }, false);

  g.setGradientFill(dialGradient);
  g.fillEllipse(area);
}

void DialComponent::drawArc(juce::Graphics& g, Dial d, juce::Colour color, float value) {
  float padding = d.arcWidth/ 2.f;
  float transformPos = PI + (PI_2 - d.arcLength) / 2.f;
  float xStart = d.xCenter - d.radius + padding;
  float yStart = d.yCenter - d.radius + padding;

  juce::Path path;
  path.addArc(xStart, yStart, d.diameter - d.arcWidth, d.diameter - d.arcWidth, 0.f, value * d.arcLength, true);

  juce::PathStrokeType arcStrokeType(d.arcWidth);
  juce::AffineTransform arcTransform = juce::AffineTransform()
    .translated(-d.xCenter, -d.yCenter)
    .rotated(transformPos)
    .translated(d.xCenter, d.yCenter);

  g.setColour(color);
  g.strokePath(path, arcStrokeType, arcTransform);
}

void DialComponent::drawOffsetCircle(juce::Graphics& g, Dial d, float value, float radius, float relativePos) {
  float thetaStart = 0.f;
  float thetaEnd = mArcLength * PI_2;
  float thetaDialPosition = (d.arcLength + (PI - d.arcLength) / 2.f) - value * d.arcLength;

  float xOffset = d.xCenter - d.radius;
  float yOffset = d.yCenter - d.radius;

  float xDial = relativePos * std::cos(thetaDialPosition);
  float yDial = relativePos * std::sin(thetaDialPosition);
  float arcWidthOffset = 1.f - 2.f * d.arcWidth / d.diameter;

  float xCirclePos = xOffset + d.radius * (1.f + xDial * arcWidthOffset);
  float yCirclePos = yOffset + d.radius * (1.f - yDial * arcWidthOffset);

  g.fillEllipse(xCirclePos - radius, yCirclePos - radius, 2.f * radius, 2.f * radius);
}

void DialComponent::resized() {
  this->setBounds(this->getLocalBounds());
}

void DialComponent::setProperty(const juce::Identifier& name, const juce::var& value) {
  jassert(!name.isNull());

  auto propName = name.toString().toStdString();
  if (propName == mProps.VALUE) {
    jassert(value.isDouble());
    mValue = value;
  }
  else if (propName == mProps.ARC_WIDTH) {
    jassert(value.isDouble());
    mArcWidth = value;
  }
  else if (propName == mProps.ARC_LENGTH) {
    jassert(value.isDouble());
    mArcLength = juce::jlimit(0.0, 2.f * juce::MathConstants<double>::pi, static_cast<double>(value));
  }
}

void DialComponent::registerView(reactjuce::ReactApplicationRoot& appRoot) {
  appRoot.registerViewType(
    "DialComponent",
    []() -> reactjuce::ViewManager::ViewPair {
      auto view = std::make_unique<DialComponent>();
      auto shadowView = std::make_unique<reactjuce::ShadowView>(view.get());

      return { std::move(view), std::move(shadowView) };
    }
  );
}
