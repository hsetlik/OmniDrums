#pragma once
#include "SampleCache.h"
#include "../Shared/RollingRMS.h"

// initialize the channel objects with a pointer to one of these such that
// they can add & remove themselves from the list of channels that need
// rendering
class PlayingChannelSet {
private:
  std::vector<int> playingChannels;

public:
  PlayingChannelSet();
  void channelStarted(int channel);
  void channelFinished(int channel);
  int numPlayingChannels() const { return (int)playingChannels.size(); }
  int operator[](int index) const { return playingChannels[(size_t)index]; }
};

// this guy holds the data from the APVTS params.
// should update once per audio block
struct channel_audio_state {
  float gainLinear = 1.0f;
  float pan = 0.5f;
  float compressorMix = 0.7f;
};

class DrumChannel {
private:
  SamplePlayer* player = nullptr;
  bool isPlaying = false;
  bool isCrossFading = false;
  size_t currentIndex = 0;
  size_t crossFadeIndex = 0;
  float currentVelocity = 0.0f;
  float nextVelocity = 0.0f;
  float lastOutput = 0.0f;

public:
  const int channel;
  RollingRMS leftRMS;
  RollingRMS rightRMS;
  channel_audio_state audioState;
  DrumChannel(int c, PlayingChannelSet* p);
  void tick();
  bool playing() const { return isPlaying; }
  void renderBlock(AudioBufF& buffer, int startSample, int numSamples);
  void renderBlock(AudioBufF& dBuf,
                   AudioBufF& cBuf,
                   int startSample,
                   int numSamples);
  void trigger(float velocity);
  void setPlayer(SamplePlayer* p) { player = p; }
  bool inUse() const { return player != nullptr; }

private:
  PlayingChannelSet* const playingSet;
};
