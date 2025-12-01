#pragma once
#include "Identifiers.h"
#include "OmniSampleLibrary.h"
#include "juce_audio_basics/juce_audio_basics.h"
#include "juce_audio_formats/juce_audio_formats.h"

class OmniState {
private:
  juce::AudioFormatManager manager;

public:
  apvts audioState;
  ValueTree samplesState;
  std::unique_ptr<OmniSampleLibrary> sampleLib;
  juce::MidiKeyboardState kbdState;

  float loadAudioParameter(const String& paramID) const;

  OmniState(juce::AudioProcessor& proc);
  juce::AudioFormatManager* getManager() { return &manager; }
  bool channelHasSample(int channelIdx) const;
};
