#include "PluginProcessor.h"
#include "PluginView.h"
#include "Constants.h"

PluginProcessor::PluginProcessor()
    : AudioProcessor(BusesProperties().withInput("Input", juce::AudioChannelSet::stereo(), true)
                                      .withOutput("Output", juce::AudioChannelSet::stereo(), true)),
      mAudioBuffer(NUM_CHANNELS, 2 * FFT_SIZE, FFT_ORDER, WINDOW_SIZE, 2, [this](juce::dsp::Complex<float>* fftData) { this->processFFT(fftData); }),
      mAudioBufferIndex{ 0 } {
  for (int index = 0; index < 12; ++index) {
    pHarmonicAmplitudes[index] = new juce::AudioParameterFloat(
      Constants::PARAM_IDS_HARMONIC_AMPLITUDE[index],
      Constants::PARAM_LABELS_HARMONIC_AMPLITUDE[index],
      0.f,
      1.f,
      1.f
    );
    this->addParameter(pHarmonicAmplitudes[index]);
  }

  pCeiling = new juce::AudioParameterFloat(Constants::PARAM_ID_CEILING, Constants::PARAM_LABEL_CEILING, 0.f, 1.f, 1.f);
  this->addParameter(pCeiling);

  pFloor = new juce::AudioParameterFloat(Constants::PARAM_ID_FLOOR, Constants::PARAM_LABEL_FLOOR, 0.f, 1.f, 1.f);
  this->addParameter(pFloor);

  pDryWet = new juce::AudioParameterFloat(Constants::PARAM_ID_DRY_WET, Constants::PARAM_LABEL_DRY_WET, 0.f, 1.f, 1.f);
  this->addParameter(pDryWet);

  pKeyBase = new juce::AudioParameterInt(Constants::PARAM_ID_KEY_BASE, Constants::PARAM_LABEL_KEY_BASE, 0, 11, 0);
  this->addParameter(pKeyBase);

  this->setLatencySamples(FFT_SIZE);
}

PluginProcessor::~PluginProcessor() {}

const juce::String PluginProcessor::getName() const {
  return JucePlugin_Name;
}

bool PluginProcessor::acceptsMidi() const {
  return false;
}

bool PluginProcessor::producesMidi() const {
  return false;
}

bool PluginProcessor::isMidiEffect() const {
  return false;
}

double PluginProcessor::getTailLengthSeconds() const {
  return 0.0;
}

int PluginProcessor::getNumPrograms() {
  return 1;
}

int PluginProcessor::getCurrentProgram() {
  return 0;
}

void PluginProcessor::setCurrentProgram(int index) {}

const juce::String PluginProcessor::getProgramName(int index) {
  return {};
}

void PluginProcessor::changeProgramName(int index, const juce::String& newName) {}

bool PluginProcessor::hasEditor() const {
  return true;
}

juce::AudioProcessorEditor* PluginProcessor::createEditor() {
  return new PluginView (*this);
}

void PluginProcessor::prepareToPlay(double sampleRate, int samplesPerBlock) {
  this->setLatencySamples(FFT_SIZE);
}

void PluginProcessor::releaseResources() {
  // When playback stops, you can use this as an opportunity to free up any
  // spare memory, etc.
}

bool PluginProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const {
  juce::AudioChannelSet channelSetIn = layouts.getMainInputChannelSet();
  juce::AudioChannelSet channelSetOut = layouts.getMainOutputChannelSet();

  bool isChannelSetMono = channelSetOut == juce::AudioChannelSet::mono();
  bool isChannelSetStereo = channelSetOut == juce::AudioChannelSet::stereo();

  // # channels out > 2 is unsupported
  if (!isChannelSetMono && !isChannelSetStereo) {
    return false;
  }

  return (channelSetOut == channelSetIn);
}

void getNoteFrequencies(float* destination) {
  for (int noteIndex = 0; noteIndex < 128; ++noteIndex) {
    destination[noteIndex] = 2. * juce::MidiMessage::getMidiNoteInHertz(noteIndex);
  }
}

void PluginProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) {
  juce::ScopedNoDenormals noDenormals;
  int numInputChannels = getTotalNumInputChannels();
  int numOutputChannels = getTotalNumOutputChannels();

  // zero out unused output channels to avoid garbage creating awful noise
  for (auto i = numInputChannels; i < numOutputChannels; ++i) {
    buffer.clear(i, 0, buffer.getNumSamples());
  }

  int numSamples = buffer.getNumSamples();
  float noteFreqs[128];
  float data[WINDOW_SIZE];
  getNoteFrequencies(noteFreqs);

  // main processing loop
  for (int channel = 0; channel < numInputChannels; ++channel) {
    float* channelData = buffer.getWritePointer(channel);

    for (int sampleIndex = 0; sampleIndex < numSamples; ++sampleIndex) {
      float outputValue = mAudioBuffer.readResult(channel);
      this->pushToFftBuffer(channel, channelData[sampleIndex]);
      channelData[sampleIndex] = outputValue;
    }
  }
}

float EPSILON = std::numeric_limits<float>::epsilon();

int getMidiNoteFromFrequency(float frequency, float middleAFrequency = 440.f) {
  //frequencyOfA * std::pow(2.0, (noteNumber - 69) / 12.0)
  float result = round(12.f * std::log2((frequency + std::numeric_limits<float>::epsilon()) / middleAFrequency)) + 69;
  return result < 0 ? 0 : result;
}

void PluginProcessor::processFFT(juce::dsp::Complex<float>* fftData) {
  float freq = 0.f;
  float hopSize = this->getSampleRate() / (float)FFT_SIZE;
  for (int bin = 0; bin < FFT_SIZE; ++bin) {
    int midiNote = getMidiNoteFromFrequency(freq);
    float scaling = std::atan2f(freq, 21500.f) * pCeiling->get();
    float amplitude = pHarmonicAmplitudes[midiNote % 12]->get();
    amplitude = juce::jmax(pFloor->get(), amplitude);
    fftData[bin] *= amplitude * std::exp((amplitude - 1.f) * scaling);
    freq += hopSize;
  }
  fftData[0] = { 0.f, 0.f };
}

void PluginProcessor::pushToFftBuffer(int channel, float sample) {
  mAudioBuffer.write(channel, sample);
}

// store state of the plugin (eg. on close)
void PluginProcessor::getStateInformation(juce::MemoryBlock& destData) {
  for (int index = 0; index < 12; ++index) {
    juce::MemoryOutputStream(destData, true).writeFloat(*pHarmonicAmplitudes[index]);
  }
}

// restore state of the plugin (eg. on re-open)
void PluginProcessor::setStateInformation(const void* data, int sizeInBytes) {
  for (int index = 0; index < 12; ++index) {
    *pHarmonicAmplitudes[index] = juce::MemoryInputStream(data, static_cast<size_t>(sizeInBytes), false).readFloat();
  }
}

// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter() {
  return new PluginProcessor();
}
