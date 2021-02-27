#include "ParametricPointComponent.h"

ParametricPointComponent::ParametricPointComponent(
  juce::Point<int> position,
  int radius,
  std::function<void(ParametricPointComponent*)> handlePointUpdate
) : mRadius(radius),
    mWidth(2 * radius),
    mHeight(2 * radius),
    mPosition(position.toFloat()),
    mOnPointUpdated(handlePointUpdate) {
  this->setBounds(mPosition.x, mPosition.y, mWidth, mHeight);
}

void ParametricPointComponent::paint(juce::Graphics& g) {
  g.setColour(mColor);
  g.fillEllipse(this->getLocalBounds().toFloat());
}

void ParametricPointComponent::resized() {
  this->setBounds(this->getBoundsInParent());
}

void ParametricPointComponent::mouseMove(const juce::MouseEvent& event) {}

void ParametricPointComponent::mouseDrag(const juce::MouseEvent& event) {
  int maxWidth = this->getParentWidth();
  int maxHeight = this->getParentHeight();
  auto mPositionPixel = mPosition.roundToInt();
  mPosition.setXY(
    static_cast<float>(juce::jlimit(-mRadius, maxWidth - 1 - mRadius, mPositionPixel.x + event.x - mRadius)),
    static_cast<float>(juce::jlimit(-mRadius, maxHeight - 1 - mRadius, mPositionPixel.y + event.y - mRadius))
  );
  this->setBounds(std::round(mPosition.x), std::round(mPosition.y), mWidth, mHeight);
  this->repaint();
  mOnPointUpdated(this);
}

void ParametricPointComponent::mouseEnter(const juce::MouseEvent& event) {
  mColor = juce::Colours::aqua;
  this->repaint();
}

void ParametricPointComponent::mouseExit(const juce::MouseEvent& event) {
  mColor = juce::Colours::beige;
  this->repaint();
}

float ParametricPointComponent::x() {
  return static_cast<float>(mPosition.x + mRadius);
}

float ParametricPointComponent::y() {
  return static_cast<float>(mPosition.y + mRadius);
}

void ParametricPointComponent::scalePosition(float x, float y) {
  mPosition.x *= x;
  mPosition.y *= y;
  this->setBounds(std::round(mPosition.x), std::round(mPosition.y), mWidth, mHeight);
  this->repaint();
}
