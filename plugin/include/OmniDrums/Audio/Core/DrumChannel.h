#pragma once
#include "SampleCache.h"

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
