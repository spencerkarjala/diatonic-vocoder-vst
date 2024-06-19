#pragma once

#include "../models/FFTBuffer.h"
#include <JuceHeader.h>


class PluginProcessor : public juce::AudioProcessor
{
  public:
    PluginProcessor();
    ~PluginProcessor() override;

    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

#ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported(const BusesLayout& layouts) const override;
#endif

    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram(int index) override;
    const juce::String getProgramName(int index) override;
    void changeProgramName(int index, const juce::String& newName) override;

    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;

    void pushToFftBuffer(int channel, float sample);

    float mOutputGain = 0.f;

    enum
    {
        FFT_ORDER = 12,
        FFT_SIZE = 1 << FFT_ORDER,
        SCOPE_SIZE = 512,
        WINDOW_SIZE = FFT_SIZE,
        NUM_CHANNELS = 2,
    };

    unsigned getBufferWritePos(unsigned channel) { return mAudioBuffer.getWritePos(channel); }

    void readAudioBuffer(float* destination, unsigned channel, unsigned endPos, unsigned length)
    {
        mAudioBuffer.readBackN(destination, channel, endPos, length);
    }

    juce::AudioParameterFloat* pHarmonicAmplitudes[12];
    juce::AudioParameterFloat* pCeiling;
    juce::AudioParameterFloat* pFloor;
    juce::AudioParameterFloat* pDryWet;
    juce::AudioParameterInt* pKeyBase;

  private:
    FFTBuffer mAudioBuffer;
    int mAudioBufferIndex[NUM_CHANNELS];

    float mFifo[FFT_SIZE];
    float mFftData[2 * FFT_SIZE];
    bool mIsNextBlockReady = false;
    float mScopeData[SCOPE_SIZE];

    void processFFT(juce::dsp::Complex<float>* fftData);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PluginProcessor)
};
