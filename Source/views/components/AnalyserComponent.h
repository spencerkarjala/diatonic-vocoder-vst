#pragma once

#include "../../controllers/PluginProcessor.h"

class AnalyserComponent : public juce::Component {
public:
  AnalyserComponent();
  ~AnalyserComponent() = default;

  void paint(juce::Graphics& g);
  void drawNextFrameOfSpectrum(juce::Graphics& g);
  void updateFFT(unsigned channel, float* fftData);

private:
  juce::Image mSpectrogramImage;
  juce::AudioBuffer<float> mFFTBuffer;
  float mScopeData[PluginProcessor::SCOPE_SIZE] = { 0 };

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AnalyserComponent);
};
