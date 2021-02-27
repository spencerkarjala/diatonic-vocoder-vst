#include "ParametricComponent.h"
#include "../helpers/ResourceHelpers.h"

ParametricComponent::ParametricComponent() {
  for (auto it : COLOR_DEFAULTS) {
    mColors.set(it.name.toString(), ResourceHelpers::parseColorString(it.value));
  }
  this->setBounds(this->getLocalBounds());
  int width = this->getWidth();
  int height = this->getHeight();
}

void ParametricComponent::paint(juce::Graphics& g) {
  juce::PathStrokeType strokeType = juce::PathStrokeType(1.f);
  juce::AffineTransform defaultTransform;

  g.setColour(mColors["grid"]);
  g.strokePath(mGrid, strokeType, defaultTransform);

  g.setColour(mColors["parametricCurve"]);
  g.strokePath(mParametricCurve, strokeType, defaultTransform);

  g.setColour(mColors["border"]);
  g.strokePath(mBorder, strokeType, defaultTransform);
}

juce::Rectangle<float> oldBounds = juce::Rectangle<float>(0.f, 0.f, 0.f, 0.f);

void ParametricComponent::resized() {
  auto newBounds = this->getLocalBounds().toFloat();
  for (auto point : mPoints) {
    if (oldBounds.getWidth() > 0 && oldBounds.getHeight() > 0) {
      point->scalePosition(
        newBounds.getWidth() / oldBounds.getWidth(),
        newBounds.getHeight() / oldBounds.getHeight()
      );
    }
  }
  
  oldBounds = newBounds;

  this->setBounds(this->getLocalBounds());

  float width = static_cast<float>(this->getWidth());
  float height = static_cast<float>(this->getHeight());
  float xCenter = width / 2.f;
  float yCenter = height / 2.f;

  mBorder.clear();
  mBorder.addLineSegment({ 0.f, 0.f, width, 0.f }, 2.f);
  mBorder.addLineSegment({ width, 0.f, width, height }, 2.f);
  mBorder.addLineSegment({ width, height, 0.f, height }, 2.f);
  mBorder.addLineSegment({ 0.f, height, 0.f, 0.f }, 2.f);

  mGrid.clear();
  float xSpacing = width / 12.f;
  for (float xPos = xSpacing; xPos < width; xPos += xSpacing) {
    mGrid.addLineSegment({ xPos, 0.f, xPos, height }, 1.f);
  }
  float ySpacing = height / 4.f;
  for (float yPos = ySpacing; yPos < height; yPos += ySpacing) {
    mGrid.addLineSegment({ 0.f, yPos, width, yPos }, 1.f);
  }

  this->drawCurves();
}

void ParametricComponent::drawCurves() {
  float width = static_cast<float>(this->getWidth());
  float yCenter = static_cast<float>(this->getHeight()) / 2.f;

  mParametricCurve.clear();
  mParametricCurve.startNewSubPath(0.f, yCenter);

  for (auto point : mPoints) {
    mParametricCurve.lineTo(point->x(), point->y());
  }

  mParametricCurve.lineTo(width, yCenter);
}

void ParametricComponent::mouseDown(const juce::MouseEvent& event) {}

void ParametricComponent::mouseDoubleClick(const juce::MouseEvent& event) {
  auto newPoint = this->createNewPoint(juce::Point<int>(event.x, event.y));
  this->addAndMakeVisible(*newPoint);

  int insertIndex;
  for (insertIndex = 0; insertIndex < mPoints.size(); ++insertIndex) {
    auto currPoint = mPoints[insertIndex];
    if (currPoint->x() >= newPoint->x()) {
      break;
    }
  }

  mPoints.insert(insertIndex, std::move(newPoint));
  this->drawCurves();
  this->repaint();
}

void ParametricComponent::mouseUp(const juce::MouseEvent& event) {}

void ParametricComponent::mouseMove(const juce::MouseEvent& event) {}

void ParametricComponent::mouseDrag(const juce::MouseEvent& event) {}

std::shared_ptr<ParametricPointComponent> ParametricComponent::createNewPoint(juce::Point<int> position) {
  return std::make_shared<ParametricPointComponent>(
    position,
    6,
    [this](ParametricPointComponent* point) -> void { this->handlePointUpdate(point); }
  );
}

void ParametricComponent::handlePointUpdate(ParametricPointComponent* point) {
  int arrayPosition = 0;
  for (auto currPoint : mPoints) {
    if (currPoint.get() == point) {
      break;
    }
    ++arrayPosition;
  }

  ParametricPointComponent* pointOnLeft = arrayPosition > 0
    ? mPoints[arrayPosition - 1].get()
    : point;
  ParametricPointComponent* pointOnRight = arrayPosition < mPoints.size() - 1
    ? mPoints[arrayPosition + 1].get()
    : point;

  if (pointOnLeft->x() > point->x()) {
    mPoints.swap(arrayPosition - 1, arrayPosition);
  }
  else if (pointOnRight->x() < point->x()) {
    mPoints.swap(arrayPosition + 1, arrayPosition);
  }
  this->drawCurves();
  this->repaint();
}

void ParametricComponent::setProperty(const juce::Identifier& name, const juce::var& value) {
  jassert(!name.isNull());
  jassert(!value.isUndefined());

  auto propName = name.toString().toStdString();
  if (propName == mProps.COLORS) {
    jassert(value.isObject());

    auto colorsProp = value.getDynamicObject();
    for (auto colorKey : COLOR_DEFAULTS) {
      auto colorName = colorKey.name.toString();
      auto colorProp = colorsProp->getProperty(colorName);

      if (colorProp.isVoid()) {
        continue;
      }
      jassert(colorProp.isString());

      mColors.set(colorName, ResourceHelpers::parseColorString(colorProp.toString()));
    }
  }
}

void ParametricComponent::registerView(reactjuce::ReactApplicationRoot& appRoot) {
  appRoot.registerViewType(
    "ParametricComponent",
    []() -> reactjuce::ViewManager::ViewPair {
      auto view = std::make_unique<ParametricComponent>();
      auto shadowView = std::make_unique<reactjuce::ShadowView>(view.get());

      return { std::move(view), std::move(shadowView) };
    }
  );
}
