#include "AnalyserComponent.h"

AnalyserComponent::AnalyserComponent()
    : mSpectrogramImage(juce::Image::RGB, 512, 512, true),
      mFFTBuffer(PluginProcessor::NUM_CHANNELS, PluginProcessor::FFT_SIZE) {}

void AnalyserComponent::paint(juce::Graphics& g) {
  g.setOpacity(1.f);
  g.drawImage(mSpectrogramImage, this->getLocalBounds().toFloat());
  this->drawNextFrameOfSpectrum(g);
}

void AnalyserComponent::drawNextFrameOfSpectrum(juce::Graphics& g) {
  int width = this->getLocalBounds().getWidth();
  int height = this->getLocalBounds().getHeight();
  int size = PluginProcessor::SCOPE_SIZE;

  for (int bin = 1; bin < size; ++bin) {
    float x0 = (float)bin - 1;
    float x1 = (float)bin;

    float y0 = (float)height - (float)height * mScopeData[bin - 1];
    float y1 = (float)height - (float)height * mScopeData[bin];

    g.setColour(juce::Colour(255, 255, 255));

    g.drawLine(x0, y0, x1, y1, 1.f);
  }
}

void AnalyserComponent::updateFFT(unsigned channel, float* fftData) {
  float* fftLocation = mFFTBuffer.getWritePointer(channel);
  memcpy(fftLocation, fftData, PluginProcessor::WINDOW_SIZE * sizeof(float));

  float minDb = -100.f;
  float maxDb = 0.f;
  float size = (float)PluginProcessor::SCOPE_SIZE;
  float halfSize = size / 2.f;
  float maxGainInDb = juce::Decibels::gainToDecibels(size);

  for (int bin = 0; bin < size; ++bin) {
    float skew = 1.f - std::pow(1.f - (float)bin / size, 0.2f);
    int fftDataIndex = juce::jlimit(0, (int)halfSize, (int)(skew * halfSize));
    float binGainInDb = juce::Decibels::gainToDecibels(fftData[fftDataIndex]);
    float binGainTruncated = juce::jlimit(minDb, maxDb, binGainInDb - maxGainInDb);
    float level = juce::jmap(binGainTruncated, minDb, maxDb, 0.f, 1.f);

    mScopeData[bin] = level;
  }

  this->repaint();
}
