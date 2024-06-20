#pragma once

#include <JuceHeader.h>

class FFTBuffer
{
  public:
    FFTBuffer(unsigned numChannels,
              unsigned size,
              unsigned fftSize,
              unsigned windowSize,
              unsigned numOverlaps,
              std::function<void(std::complex<float>*, unsigned int)> processFFT);
    ~FFTBuffer() = default;

    void write(unsigned channel, float sample);
    float readResult(unsigned channel);
    unsigned getWritePos(unsigned channel);

  private:
    juce::AudioBuffer<float> m_inputAudio;
    std::vector<juce::AudioBuffer<juce::dsp::Complex<float>>> m_outputAudioFrames;
    juce::AudioBuffer<std::complex<float>> mResultBuffer;
    std::vector<std::atomic<long unsigned>> mReadPos, mWritePos;
    std::vector<std::atomic<long unsigned>> mResultReadPos;
    juce::dsp::FFT mFFT;
    juce::dsp::WindowingFunction<float> mWindow;

    std::vector<unsigned> mFrameIndex;

    std::vector<std::queue<float>> mResults;

    std::function<void(std::complex<float>*, unsigned int)> mProcessFFT;

    std::atomic<bool> m_minSamplesReached = false;

    unsigned int m_size;
    unsigned int m_sizeWindow;
    unsigned int m_sizeOverlaps;
    unsigned int m_numOverlaps;

    unsigned int getThenIncrementWrite(unsigned channel, unsigned num = 1);
    unsigned int getThenIncrementRead(unsigned channel, unsigned num = 1);
    unsigned int getThenIncrementReadResult(unsigned channel, unsigned num = 1);
    void performFFT(const unsigned channel);
};
