#pragma once
#include <JuceHeader.h>
#include "react_juce.h"

class FloorCeilingSliderComponent : public reactjuce::View {
public:
  FloorCeilingSliderComponent();
  ~FloorCeilingSliderComponent() = default;

  void paint(juce::Graphics& g) override;
  void resized() override;
  void mouseDown(const juce::MouseEvent& event) override;
  void mouseUp(const juce::MouseEvent& event) override;
  void mouseDrag(const juce::MouseEvent& event) override;

  static void registerView(reactjuce::ReactApplicationRoot& appRoot);
  void setProperty(const juce::Identifier& name, const juce::var& value);

private:
  juce::DrawableComposite mDrawables;
  std::unique_ptr<juce::Drawable> mDecalBackground;
  std::unique_ptr<juce::Drawable> mDecalForeground;
  std::unique_ptr<juce::Drawable> mSliderTop;
  std::unique_ptr<juce::Drawable> mSliderBottom;

  enum MouseSelections {
    SELECTED_NONE, SELECTED_CEILING, SELECTED_FLOOR,
  };

  float mCeiling = 1.f;
  float mFloor = 0.f;
  int mSelected = SELECTED_NONE;


  void drawDecals(juce::Graphics& g);

  const struct ReactProps {};
  ReactProps mProps;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FloorCeilingSliderComponent)
};
