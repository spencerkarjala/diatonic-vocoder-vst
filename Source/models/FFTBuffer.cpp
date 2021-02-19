#include "FFTBuffer.h"

FFTBuffer::FFTBuffer(unsigned numChannels, size_t size, unsigned fftOrder, unsigned windowSize, unsigned numOverlaps, std::function<void(juce::dsp::Complex<float>*)> processFFT)
    : mBuffer(numChannels, static_cast<int>(size)),
      mFFTBuffer(numOverlaps),
      mResultBuffer(numChannels, windowSize),
      mResults(numChannels),
      mFFT(fftOrder),
      mWindow(windowSize, juce::dsp::WindowingFunction<float>::hann, false),
      mSize(static_cast<int>(size)),
      mSizeFFT(1 << fftOrder),
      mSizeWindow(windowSize),
      mSizeOverlaps(windowSize / numOverlaps),
      mNumOverlaps(numOverlaps),
      mReadPos(numChannels),
      mWritePos(numChannels),
      mResultReadPos(numChannels),
      tmp(windowSize),
      tmp2(windowSize),
      mProcessFFT(processFFT),
      mFrameIndex(numChannels) {
  mBuffer.clear();
  for (unsigned int channel = 0; channel < numChannels; ++channel) {
    for (unsigned int sample = 0; sample < windowSize; ++sample) {
      mResultBuffer.setSample(channel, sample, { 0.f, 0.f });
    }
  }
  for (unsigned int overlap = 0; overlap < numOverlaps; ++overlap) {
    mFFTBuffer[overlap] = juce::AudioBuffer<juce::dsp::Complex<float>>(numChannels, windowSize);
    for (unsigned int channel = 0; channel < numChannels; ++channel) {
      for (unsigned int sample = 0; sample < windowSize; ++sample) {
        mFFTBuffer[overlap].setSample(channel, sample, { 0.f, 0.f });
      }
    }
  }
  for (unsigned channel = 0; channel < numChannels; ++channel) {
    mFrameIndex[channel] = 0;
  }
}

void FFTBuffer::write(unsigned channel, float sample) {
  mBuffer.setSample(channel, this->getThenIncrementWrite(channel), sample);
  if (mWritePos[channel] % mSizeOverlaps == 0) {
    this->performFFT(channel);
  }
}

void FFTBuffer::read(float* destination, unsigned channel, unsigned length) {
  unsigned endPos = mWritePos[channel];
  this->readBackN(destination, channel, endPos, length);
}

void FFTBuffer::readBackN(float* destination, unsigned channel, unsigned endPos, unsigned length) {
  unsigned startPos = (endPos - length) % mSize;
  float* source = mBuffer.getWritePointer(channel, startPos);
  if (endPos > startPos) {
    memcpy(destination, source, length * sizeof(float));
  }
  else {
    size_t firstChunkSize = (mSize - startPos) * sizeof(float);
    size_t lastChunkSize = (length * sizeof(float)) - firstChunkSize;
    memcpy(destination, source, firstChunkSize);
    source = mBuffer.getWritePointer(channel);
    memcpy(destination + firstChunkSize, source, lastChunkSize);
  }
}

float FFTBuffer::readResult(unsigned channel) {
  //unsigned sampleIndex = this->getThenIncrementReadResult(channel);
  //return mResultBuffer.getSample(channel, sampleIndex).real();
  if (mResults[channel].empty()) {
    return 0.f;
  }
  float result = mResults[channel].front();
  mResults[channel].pop();
  return result;
}

unsigned FFTBuffer::getWritePos(unsigned channel) {
  return mWritePos[channel];
}

unsigned FFTBuffer::getThenIncrementWrite(unsigned channel, unsigned num) {
  long unsigned prev = mWritePos[channel];
  mWritePos[channel] = (prev + num >= mSize ? 0 : prev + num);
  return prev;
}

unsigned FFTBuffer::getThenIncrementRead(unsigned channel, unsigned num) {
  long unsigned prev = mReadPos[channel];
  mReadPos[channel] = (prev + num == mSize ? 0 : prev + num);
  return prev;
}

unsigned FFTBuffer::getThenIncrementReadResult(unsigned channel, unsigned num) {
  long unsigned prev = mResultReadPos[channel];
  mResultReadPos[channel] = (prev + num == mSizeWindow ? 0 : prev + num);
  return prev;
}

void FFTBuffer::performFFT(unsigned channel) {
  const float* audioData = mBuffer.getReadPointer(channel);
  juce::dsp::Complex<float>* fftData = mFFTBuffer[mFrameIndex[channel]].getWritePointer(channel);
  juce::dsp::Complex<float>* resultData = mResultBuffer.getWritePointer(channel);
  unsigned bufferPos = mWritePos[channel].load();
  unsigned bufferBase = (bufferPos - mSizeWindow) % mSize;

  for (unsigned int sample = 0; sample < mSizeWindow; ++sample) {
    tmp[sample] = audioData[(bufferBase + sample) % mSize];
  }
  mWindow.multiplyWithWindowingTable(&tmp[0], mSizeWindow);

  for (unsigned int sample = 0; sample < mSizeWindow; ++sample) {
    resultData[sample] = { tmp[sample], 0.f };
  }

  mFFT.perform(resultData, &tmp2[0], false);
  this->mProcessFFT(&tmp2[0]);
  mFFT.perform(&tmp2[0], fftData, true);

  unsigned frameIndexBase = mFrameIndex[channel];
  juce::dsp::Complex<float> result;
  for (unsigned int sample = 0; sample < mSizeWindow; ++sample) {
    result = 0;
    for (unsigned int frame = 0; frame < mNumOverlaps; ++frame) {
      if (sample + frame * mSizeOverlaps < mSizeWindow) {
        result += mFFTBuffer[(frameIndexBase + frame) % mNumOverlaps].getSample(channel, sample + frame * mSizeOverlaps);
      }
    }
    resultData[sample] = result / (float)(mNumOverlaps > 1 ? mNumOverlaps / 2 : 1);
  }

  for (unsigned int sample = 0; sample < mSizeOverlaps; ++sample) {
    mResults[channel].push(resultData[sample].real());
  }

  mFrameIndex[channel] = (mFrameIndex[channel] + 1) % mNumOverlaps;
}
