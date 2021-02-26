#pragma once
#include <JuceHeader.h>
#include "react_juce.h"

class SVGComponent : public reactjuce::View {
public:
  SVGComponent();
  SVGComponent(const char* source);
  ~SVGComponent() = default;

  void paint(juce::Graphics& g) override;
  void resized() override;

  // reactjuce::View method overrides
  void setProperty(const juce::Identifier& name, const juce::var& value) override;
  void addChild(View* childView, int index) override;

  static void registerView(reactjuce::ReactApplicationRoot& appRoot);

private:
  std::unique_ptr<juce::Drawable> mDrawable;
  juce::ComponentBoundsConstrainer mConstrainer;
  juce::Array<juce::Colour> mColorReplacements;
  juce::RectanglePlacement mAlignment = juce::RectanglePlacement::centred;

  const struct ReactProps {
    std::string SOURCE = std::string("source");
    std::string DATA = std::string("data");
    std::string COLOR_REPLACEMENTS = std::string("colorReplacements");
    std::string ALIGNMENT = std::string("alignment");
  };
  ReactProps mProps;

  const struct Alignments {
    std::string X_LEFT = std::string("xLeft");
    std::string X_RIGHT = std::string("xRight");
    std::string X_MID = std::string("xMid");
    std::string Y_TOP = std::string("yTop");
    std::string Y_BOTTOM = std::string("yBottom");
    std::string Y_MID = std::string("yMid");
    std::string CENTRED = std::string("centred");
  };
  Alignments mAlignments;

  juce::Array<Component*> mChildren;

  void loadSVGResource(const std::string source);
  void loadSVGData(const std::string data);
  void updateDrawColors();

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SVGComponent)
};
