#pragma once
#include "../../OmniState.h"
#include "OmniDrums/Identifiers.h"
#include "SampleCache.h"
#include "DrumChannel.h"
#include "juce_audio_basics/juce_audio_basics.h"
#include "juce_events/juce_events.h"

// this handles playback of the previewed samples from the browser

class PreviewPlayer : juce::AsyncUpdater {
public:
  enum PlaybackStage { Idle, WaitingOnPlayer, Playing };

private:
  OmniState* const state;
  PlaybackStage currentStage = Idle;
  std::unique_ptr<SamplePlayer> player;
  juce::File sampleFile;
  int lengthInSamples = 0;
  int currentSample = 0;

public:
  PreviewPlayer(OmniState* s);
  // call this to start playback
  void previewSample(juce::File& file);
  void handleAsyncUpdate() override;
  // audio thread checks this to see if we have a preview to
  // play
  bool isPlaying() const { return currentStage == Playing; }
  void renderPreview(AudioBufF& dryBuf,
                     AudioBufF& compBuf,
                     int startSample,
                     int numSamples);
};

//=========================================================================

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
  PreviewPlayer prevPlayer;

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
