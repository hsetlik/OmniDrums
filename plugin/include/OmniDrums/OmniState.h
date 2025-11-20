#pragma once
#include "Identifiers.h"
#include "OmniSampleLibrary.h"
#include "juce_audio_basics/juce_audio_basics.h"

class OmniState {
public:
  apvts audioState;
  ValueTree samplesState;
  OmniSampleLibrary sampleLib;
  juce::MidiKeyboardState kbdState;

  float loadAudioParameter(const String& paramID) const;

  OmniState(juce::AudioProcessor& proc);
};
