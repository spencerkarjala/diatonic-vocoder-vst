#include <cassert>

#include "FFTBuffer.h"

FFTBuffer::FFTBuffer(unsigned numChannels,
                     unsigned size,
                     unsigned fftOrder,
                     unsigned windowSize,
                     unsigned numOverlaps,
                     std::function<void(std::complex<float>*, unsigned int)> processFFT)
  : m_inputAudio(numChannels, size)
  , m_outputAudioFrames(numOverlaps)
  , mResultBuffer(numChannels, windowSize)
  , mResults(numChannels)
  , mFFT(fftOrder)
  , mWindow(windowSize, juce::dsp::WindowingFunction<float>::hann, false)
  , m_size(static_cast<unsigned int>(size))
  , m_sizeWindow(windowSize)
  , m_sizeOverlaps(windowSize / numOverlaps)
  , m_numOverlaps(numOverlaps)
  , mReadPos(numChannels)
  , mWritePos(numChannels)
  , mResultReadPos(numChannels)
  , mProcessFFT(processFFT)
  , mFrameIndex(numChannels)
{
    for (unsigned int channel = 0; channel < numChannels; ++channel) {
        for (unsigned int sample = 0; sample < windowSize; ++sample) {
            mResultBuffer.setSample(channel, sample, { 0.f, 0.f });
        }
    }

    for (unsigned int channel = 0; channel < numChannels; ++channel) {
        for (unsigned int sample = 0; sample < windowSize; ++sample) {
            m_inputAudio.setSample(channel, sample, 0.f);
        }
    }

    for (unsigned int frame = 0; frame < numOverlaps; ++frame) {
        m_outputAudioFrames[frame] = juce::AudioBuffer<juce::dsp::Complex<float>>(numChannels, windowSize);

        for (unsigned int channel = 0; channel < numChannels; ++channel) {
            for (unsigned int sample = 0; sample < windowSize; ++sample) {
                m_outputAudioFrames[frame].setSample(channel, sample, { 0.f, 0.f });
            }
        }
    }

    for (unsigned channel = 0; channel < numChannels; ++channel) {
        mFrameIndex[channel] = 0;
    }
}

void FFTBuffer::write(unsigned channel, float sample)
{
    const auto oldWritePos = mWritePos[channel].load();

    const unsigned sampleIndex = this->getThenIncrementWrite(channel);
    m_inputAudio.setSample(channel, sampleIndex, sample);

    if (mWritePos[channel] < oldWritePos && !m_minSamplesReached) {
        m_minSamplesReached.store(true);
    }

    if (mWritePos[channel] % m_sizeOverlaps == 0 && m_minSamplesReached) {
        this->performFFT(channel);
    }
}

float FFTBuffer::readResult(unsigned channel)
{
    if (mResults[channel].empty()) {
        return 0.f;
    }
    float result = mResults[channel].front();
    mResults[channel].pop();
    return result;
}

unsigned FFTBuffer::getWritePos(unsigned channel)
{
    return mWritePos[channel];
}

unsigned FFTBuffer::getThenIncrementWrite(unsigned channel, unsigned num)
{
    long unsigned prev = mWritePos[channel];
    mWritePos[channel] = (prev + num >= m_size ? 0 : prev + num);
    return prev;
}

unsigned FFTBuffer::getThenIncrementRead(unsigned channel, unsigned num)
{
    long unsigned prev = mReadPos[channel];
    mReadPos[channel] = (prev + num == m_sizeWindow ? 0 : prev + num);
    return prev;
}

unsigned FFTBuffer::getThenIncrementReadResult(unsigned channel, unsigned num)
{
    long unsigned prev = mResultReadPos[channel];
    mResultReadPos[channel] = (prev + num == m_sizeWindow ? 0 : prev + num);
    return prev;
}

void FFTBuffer::performFFT(const unsigned channel)
{
    const float* audioInputData = m_inputAudio.getReadPointer(channel);
    unsigned int bufferPos = mWritePos[channel].load();
    unsigned int bufferBase = (bufferPos - m_sizeWindow) % m_size;

    std::vector<float> windowedAudioData(m_sizeWindow);

    for (unsigned int sample = 0; sample < m_sizeWindow; ++sample) {
        unsigned int circularIndex = (bufferBase + sample) % m_size;
        windowedAudioData[sample] = audioInputData[circularIndex];
    }

    std::vector<std::complex<float>> fftInput(m_sizeWindow);
    std::vector<std::complex<float>> spectrumData(m_sizeWindow);
    std::vector<std::complex<float>> fftOutput(m_sizeWindow);

    for (unsigned int sample = 0; sample < m_sizeWindow; ++sample) {
        fftInput[sample] = { windowedAudioData[sample], 0.f };
    }

    mFFT.perform(&fftInput[0], &spectrumData[0], false);
    this->mProcessFFT(&spectrumData[0], channel);
    mFFT.perform(&spectrumData[0], &fftOutput[0], true);

    std::vector<float> realData(m_sizeWindow);

    for (unsigned int sample = 0; sample < m_sizeWindow; ++sample) {
        realData[sample] = fftOutput[sample].real();
    }

    mWindow.multiplyWithWindowingTable(&realData[0], m_sizeWindow);

    std::complex<float>* fftData = m_outputAudioFrames[mFrameIndex[channel]].getWritePointer(channel);

    for (unsigned int sample = 0; sample < m_sizeWindow; ++sample) {
        fftData[sample] = { realData[sample], 0.f };
    }

    std::complex<float>* resultData = mResultBuffer.getWritePointer(channel);
    std::complex<float> result;
    unsigned frameIndexBase = mFrameIndex[channel];

    for (unsigned int sample = 0; sample < m_sizeWindow / m_numOverlaps; ++sample) {
        result = { 0.f, 0.f };

        for (unsigned int frame = 0; frame < m_numOverlaps; ++frame) {
            unsigned frameIndex = (frameIndexBase + frame) % m_numOverlaps;
            unsigned sampleIndex = sample + frame * m_sizeOverlaps;

            auto& outputAudioFrame = m_outputAudioFrames[frameIndex];
            result += outputAudioFrame.getSample(channel, sampleIndex);
        }

        resultData[sample] = result;
    }

    for (unsigned int sample = 0; sample < m_sizeOverlaps; ++sample) {
        mResults[channel].push(resultData[sample].real());
    }

    mFrameIndex[channel] = (mFrameIndex[channel] + 1) % m_numOverlaps;
}
