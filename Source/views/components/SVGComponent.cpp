#include "SVGComponent.h"
#include "../helpers/ResourceHelpers.h"

SVGComponent::SVGComponent() {
  mDrawable = nullptr;
}

SVGComponent::SVGComponent(const char* source) {}

void SVGComponent::paint(juce::Graphics& g) {
  jassert(mDrawable != nullptr);
}

void SVGComponent::resized() {
  jassert(mDrawable != nullptr);

  auto bounds = getLocalBounds().toFloat();
  mDrawable->setTransformToFit(bounds, mAlignment);
}

void SVGComponent::setProperty(const juce::Identifier& name, const juce::var& value) {
  jassert(!name.isNull());
  jassert(!value.isUndefined());

  if (value.isUndefined()) {
    return;
  }

  auto propName = name.toString().toStdString();
  if (propName == mProps.SOURCE) {
    jassert(value.isString());
    this->loadSVGResource(value.toString().toStdString());
  }
  else if (propName == mProps.DATA) {
    jassert(value.isString());
    this->loadSVGData(value.toString().toStdString());
  }
  else if (propName == mProps.COLOR_REPLACEMENTS) {
    jassert(value.isArray());
    jassert(value.size() % 2 == 0);

    mColorReplacements.clear();
    for (auto stringVar : *value.getArray()) {
      jassert(stringVar.isString());

      auto color = ResourceHelpers::parseColorString(stringVar.toString());
      mColorReplacements.add(color);
    }

    if (mDrawable) {
      this->updateDrawColors();
    }
  }
  else if (propName == mProps.ALIGNMENT) {
    jassert(value.isString());

    std::string alignment = value.toString().toStdString();
    if (alignment == mAlignments.X_LEFT) {
      mAlignment = juce::RectanglePlacement::xLeft;
    }
    else if (alignment == mAlignments.X_RIGHT) {
      mAlignment = juce::RectanglePlacement::xRight;
    }
    else if (alignment == mAlignments.X_MID) {
      mAlignment = juce::RectanglePlacement::xMid;
    }
    else if (alignment == mAlignments.Y_TOP) {
      mAlignment = juce::RectanglePlacement::yTop;
    }
    else if (alignment == mAlignments.Y_BOTTOM) {
      mAlignment = juce::RectanglePlacement::yBottom;
    }
    else if (alignment == mAlignments.Y_MID) {
      mAlignment = juce::RectanglePlacement::yMid;
    }
    else if (alignment == mAlignments.CENTRED) {
      mAlignment = juce::RectanglePlacement::centred;
    }
  }
}

void SVGComponent::addChild(View* childView, int index) {
  mChildren.add(childView);
}

void SVGComponent::loadSVGResource(const std::string source) {
  juce::File svgFile(ResourceHelpers::getSVGPath(source).string());
  mDrawable = juce::Drawable::createFromImageFile(svgFile);

  this->addAndMakeVisible(mDrawable.get());

  this->updateDrawColors();

  for (auto child : mChildren) {
    this->addAndMakeVisible(child);
  }
}

void SVGComponent::loadSVGData(const std::string data) {
  mDrawable = juce::Drawable::createFromImageData(data.c_str(), data.length());

  auto children = this->getChildren();
  this->removeAllChildren();

  for (auto child : children) {
    mDrawable->addAndMakeVisible(child);
  }
}

void SVGComponent::updateDrawColors() {
  jassert(mDrawable);
  jassert(mColorReplacements.size() % 2 == 0);

  for (int index = 0; index < mColorReplacements.size(); index += 2) {
    mDrawable->replaceColour(mColorReplacements[index], mColorReplacements[index + 1]);
  }
}

void SVGComponent::registerView(reactjuce::ReactApplicationRoot& appRoot) {
  appRoot.registerViewType(
    "SVGImage",
    []() -> reactjuce::ViewManager::ViewPair {
      auto view = std::make_unique<SVGComponent>();
      auto shadowView = std::make_unique<reactjuce::ShadowView>(view.get());

      return { std::move(view), std::move(shadowView) };
    }
  );
}
