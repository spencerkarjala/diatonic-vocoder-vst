#pragma once

#include <JuceHeader.h>

class FFTBuffer {
public:
  FFTBuffer(unsigned numChannels, size_t size, unsigned fftSize, unsigned windowSize, unsigned numOverlaps, std::function<void(juce::dsp::Complex<float>*)> processFFT);
  ~FFTBuffer() = default;

  void write(unsigned channel, float sample);
  void read(float* destination, unsigned channel, unsigned length);
  void readBackN(float* destination, unsigned channel, unsigned endpos, unsigned length);
  float readResult(unsigned channel);
  unsigned getWritePos(unsigned channel);

  class ComplexPolar {
  public:
    ComplexPolar()
      : r(0.f),
        a(0.f) {}
    ComplexPolar(float magnitude, float theta)
      : r(magnitude),
        a(theta) {}
    ComplexPolar(std::complex<float> z)
      : r(std::abs(z)),
        a(std::arg(z)) {}

    float r, a;
  };

private:
  juce::AudioBuffer<float> mBuffer;
  std::vector<juce::AudioBuffer<juce::dsp::Complex<float>>> mFFTBuffer;
  juce::AudioBuffer<juce::dsp::Complex<float>> mResultBuffer;
  std::vector<std::atomic<long unsigned>> mReadPos, mWritePos;
  std::vector<std::atomic<long unsigned>> mResultReadPos;
  size_t mSize;
  juce::dsp::FFT mFFT;
  juce::dsp::WindowingFunction<float> mWindow;

  std::vector<float> tmp;
  std::vector<juce::dsp::Complex<float>> tmp2;
  std::vector<unsigned> mFrameIndex;

  std::vector<std::queue<float>> mResults;

  std::function<void(juce::dsp::Complex<float>*)> mProcessFFT;

  unsigned mSizeFFT;
  unsigned mSizeWindow;
  unsigned mSizeOverlaps;
  unsigned mNumOverlaps;

  unsigned getThenIncrementWrite(unsigned channel, unsigned num = 1);
  unsigned getThenIncrementRead(unsigned channel, unsigned num = 1);
  unsigned getThenIncrementReadResult(unsigned channel, unsigned num = 1);
  void performFFT(unsigned channel);
};
