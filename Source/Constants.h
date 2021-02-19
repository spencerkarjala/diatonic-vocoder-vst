#pragma once

#include <JuceHeader.h>

namespace Constants {
  const juce::StringArray KEY_LABELS = {
  "A\0", "A#\0", "B\0", "C\0", "C#\0", "D\0", "D#\0", "E\0", "F\0", "F#\0", "G\0", "G#\0",
  };
  const juce::StringArray SCALE_LABELS = {
    "Major", "Minor", "Minor Pentatonic",
  };
  const std::array<std::array<float, 12>, 3> SCALE_AMPLITUDES = {
    std::array<float, 12>({ 1.f, 0.f, 1.f, 0.f, 1.f, 1.f, 0.f, 1.f, 0.f, 1.f, 0.f, 1.f }),
    std::array<float, 12>({ 1.f, 0.f, 1.f, 1.f, 0.f, 1.f, 0.f, 1.f, 1.f, 0.f, 1.f, 0.f }),
    std::array<float, 12>({ 1.f, 0.f, 0.f, 1.f, 0.f, 1.f, 0.f, 1.f, 0.f, 0.f, 1.f, 0.f }),
  };

  const std::string PARAM_IDS_HARMONIC_AMPLITUDE[12] = {
    "C-harmonic-amplitude",
    "C#-harmonic-amplitude",
    "D-harmonic-amplitude",
    "D#-harmonic-amplitude",
    "E-harmonic-amplitude",
    "F-harmonic-amplitude",
    "F#-harmonic-amplitude",
    "G-harmonic-amplitude",
    "G#-harmonic-amplitude",
    "A-harmonic-amplitude",
    "A#-harmonic-amplitude",
    "B-harmonic-amplitude",
  };

  const std::string PARAM_LABELS_HARMONIC_AMPLITUDE[12] = {
    "C Amplitude",
    "C# Amplitude",
    "D Amplitude",
    "D# Amplitude",
    "E Amplitude",
    "F Amplitude",
    "F# Amplitude",
    "G Amplitude",
    "G# Amplitude",
    "A Amplitude",
    "A# Amplitude",
    "B Amplitude",
  };

  const std::string PARAM_ID_CEILING = "param-ceiling";
  const std::string PARAM_LABEL_CEILING = "Ceiling";

  const std::string PARAM_ID_FLOOR = "param-floor";
  const std::string PARAM_LABEL_FLOOR = "Floor";

  const std::string PARAM_ID_DRY_WET = "param-dry-wet";
  const std::string PARAM_LABEL_DRY_WET = "Dry/Wet";

  const std::string PARAM_ID_KEY_BASE = "param-key-base";
  const std::string PARAM_LABEL_KEY_BASE = "Key";
}
