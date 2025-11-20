#pragma once
#include "SampleCache.h"

// this guy holds the data from the APVTS params.
// should update once per audio block
struct channel_audio_state {
  float gainLinear = 1.0f;
  float panLinear = 0.5f;
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
  DrumChannel(int c);
  void tick();
  float getOutput() const { return lastOutput; }
  void trigger(float velocity);
  void setPlayer(SamplePlayer* p) { player = p; }
};
