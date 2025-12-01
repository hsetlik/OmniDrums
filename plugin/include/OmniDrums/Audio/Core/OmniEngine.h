#pragma once
#include "../../OmniState.h"
#include "OmniDrums/Identifiers.h"
#include "SampleCache.h"
#include "DrumChannel.h"
#include "juce_audio_basics/juce_audio_basics.h"

typedef std::array<int, MAX_MIDI_NUM> note_channel_map;

class OmniEngine {
private:
  OmniState* const state;
  SampleCache sampleCache;
  PlayingChannelSet playingChannels;
  juce::OwnedArray<DrumChannel> drumChannels;
  note_channel_map noteMap;
  int minimunBlockSize = 32;
  int currentBlockSize = 128;
  AudioBufF dryBuffer;
  AudioBufF compBuffer;

public:
  OmniEngine(OmniState* s);
  void setBlockSize(int newSize);
  void renderBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiBuf);

private:
  juce::CriticalSection cs;
  void prepareForBlock();
  void handleMidiMessage(const juce::MidiMessage& msg);
  void renderChannels(AudioBufF& buf, int startSample, int numSamples);
  void renderChannels(AudioBufF& dBuf,
                      AudioBufF& cBuf,
                      int startSample,
                      int numSamples);
};
