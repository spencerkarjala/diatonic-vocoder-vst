#include "FloorCeilingSliderComponent.h"
#include "../helpers/ResourceHelpers.h"

FloorCeilingSliderComponent::FloorCeilingSliderComponent() {
  juce::File svgFile(ResourceHelpers::getSVGPath("floor-ceil-slider-bg.svg").string());
  mDecalBackground = juce::Drawable::createFromImageFile(svgFile);
  mDecalBackground->replaceColour(juce::Colours::black, juce::Colour(0xff444444));

  mDecalForeground = juce::Drawable::createFromSVGFile(svgFile);
  mDecalForeground->replaceColour(juce::Colours::black, juce::Colour(0xfffafaaa));

  svgFile = juce::File(ResourceHelpers::getSVGPath("floor-ceil-slider-top.svg").string());
  mSliderTop = juce::Drawable::createFromSVGFile(svgFile);
  mSliderTop->replaceColour(juce::Colours::black, juce::Colour(0xffaabaca));

  svgFile = juce::File(ResourceHelpers::getSVGPath("floor-ceil-slider-bottom.svg").string());
  mSliderBottom = juce::Drawable::createFromSVGFile(svgFile);
  mSliderBottom->replaceColour(juce::Colours::black, juce::Colour(0xff3a7aaa));

  auto bounds = mDecalBackground->getDrawableBounds();
  mDecalBackground->centreWithSize(juce::roundToInt(bounds.getWidth()), juce::roundToInt(bounds.getHeight()));
  bounds = mDecalForeground->getDrawableBounds();
  mDecalForeground->centreWithSize(juce::roundToInt(bounds.getWidth()), juce::roundToInt(bounds.getHeight()));
  bounds = mSliderTop->getDrawableBounds();
  mSliderTop->centreWithSize(juce::roundToInt(bounds.getWidth()), juce::roundToInt(bounds.getHeight()));
  bounds = mSliderBottom->getDrawableBounds();
  mSliderBottom->centreWithSize(juce::roundToInt(bounds.getWidth()), juce::roundToInt(bounds.getHeight()));
}

void FloorCeilingSliderComponent::paint(juce::Graphics& g) {
  this->drawDecals(g);

  auto localBounds = this->getLocalBounds();
  auto sliderLocalBounds = mSliderTop->getDrawableBounds();

  float widthScaling = localBounds.getWidth() / sliderLocalBounds.getWidth();
  sliderLocalBounds.setWidth(localBounds.getWidth());
  sliderLocalBounds.setHeight(sliderLocalBounds.getHeight() * widthScaling);
  sliderLocalBounds.setCentre({ localBounds.getWidth() / 2.f, localBounds.getHeight() * (1.f - mCeiling) });

  mSliderTop->drawWithin(g, sliderLocalBounds, juce::RectanglePlacement::centred, 1.f);

  sliderLocalBounds.setCentre({ localBounds.getWidth() / 2.f, localBounds.getHeight() * (1.f - mFloor) });

  mSliderBottom->drawWithin(g, sliderLocalBounds, juce::RectanglePlacement::centred, 1.f);
}

void FloorCeilingSliderComponent::drawDecals(juce::Graphics& g) {
  auto bounds = this->getLocalBounds().toFloat();
  float decalLocalHeight = mDecalForeground->getDrawableBounds().getHeight();

  float clipTop = decalLocalHeight * (1.f - mCeiling);
  float clipBottom = decalLocalHeight * (1.f - mFloor);

  auto topClipPath = juce::Path();
  topClipPath.addRectangle(0.f, clipBottom, static_cast<float>(this->getWidth()), clipTop - clipBottom);
  std::unique_ptr<juce::DrawablePath> clip = std::make_unique<juce::DrawablePath>();
  clip->setPath(topClipPath);
  mDecalForeground->setClipPath(std::move(clip));

  mDecalBackground->drawWithin(g, bounds, juce::RectanglePlacement::centred, 1.f);
  mDecalForeground->drawWithin(g, bounds, juce::RectanglePlacement::centred, 1.f);
}

void FloorCeilingSliderComponent::resized() {
  this->setBounds(this->getLocalBounds());
  mSliderTop->setBoundsToFit(this->getLocalBounds(), juce::Justification::centred, false);
}

void FloorCeilingSliderComponent::mouseDown(const juce::MouseEvent& event) {
  float yNorm = 1.f - static_cast<float>(event.y) / this->getHeight();

  if (yNorm > mCeiling - 0.1f && yNorm < mFloor + 0.1f) {
    return;
  }
  else if (yNorm > mCeiling - 0.1f) {
    mSelected = SELECTED_CEILING;
  }
  else if (yNorm < mFloor + 0.1f) {
    mSelected = SELECTED_FLOOR;
  }
}

void FloorCeilingSliderComponent::mouseUp(const juce::MouseEvent& event) {
  mSelected = SELECTED_NONE;
}

void FloorCeilingSliderComponent::mouseDrag(const juce::MouseEvent & event) {
  float yNorm = 1.f - static_cast<float>(event.y) / this->getHeight();
  if (mSelected == SELECTED_CEILING) {
    mCeiling = juce::jlimit(mFloor, 1.f, yNorm);
  }
  else if (mSelected == SELECTED_FLOOR) {
    mFloor = juce::jlimit(0.f, mCeiling, yNorm);
  }
  this->repaint();
}

void FloorCeilingSliderComponent::setProperty(const juce::Identifier& name, const juce::var& value) {
  jassert(!name.isNull());
}

void FloorCeilingSliderComponent::registerView(reactjuce::ReactApplicationRoot& appRoot) {
  appRoot.registerViewType(
    "FloorCeilingSliderComponent",
    []() -> reactjuce::ViewManager::ViewPair {
      auto view = std::make_unique<FloorCeilingSliderComponent>();
      auto shadowView = std::make_unique<reactjuce::ShadowView>(view.get());

      return { std::move(view), std::move(shadowView) };
    }
  );
}
