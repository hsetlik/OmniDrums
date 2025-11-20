#pragma once
#include "../../OmniState.h"
#include "OmniDrums/Identifiers.h"
#include "SampleCache.h"
#include "DrumChannel.h"
#include "juce_audio_basics/juce_audio_basics.h"

class OmniEngine {
private:
  struct timed_midi_msg {
    int sampleIdx;
    juce::MidiMessage message;
  };
  OmniState* const state;
  SampleCache sampleCache;
  juce::OwnedArray<DrumChannel> drumChannels;
  std::queue<timed_midi_msg> midiQueue;

public:
  OmniEngine(OmniState* s);
  void renderBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiBuf);

private:
  void prepareForBlock();
  void loadMidiMessages(juce::MidiBuffer& midiBuf, int bufLength);
};
