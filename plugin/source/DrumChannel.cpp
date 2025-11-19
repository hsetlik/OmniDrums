#include "OmniDrums/Audio/Core/DrumChannel.h"

#define CROSS_FADE_SAMPLES 100
//===================================================
DrumChannel::DrumChannel(int c) : channel(c) {}

void DrumChannel::tick() {
  if (!isPlaying)
    return;
  if (!isCrossFading) {
    lastOutput = player->getValue(currentIndex);
    ++currentIndex;
    isPlaying = currentIndex < player->lengthInSamples();
  } else {
    auto prevIdx = juce::jmin(currentIndex + crossFadeIndex,
                              player->lengthInSamples() - 1);
    float ratio = (float)crossFadeIndex / (float)CROSS_FADE_SAMPLES;
    lastOutput = flerp(player->getValue(prevIdx),
                       player->getValue(crossFadeIndex), ratio);
    ++crossFadeIndex;
    // check if we're finished cross fading yet
    if (crossFadeIndex >= CROSS_FADE_SAMPLES) {
      currentIndex = crossFadeIndex;
      currentVelocity = nextVelocity;
      isCrossFading = false;
    }
  }
}

void DrumChannel::trigger(float velocity) {
  if (player == nullptr)
    return;
  if (!isPlaying) {
    isPlaying = true;
    isCrossFading = false;
    currentVelocity = velocity;
    currentIndex = 0;
    lastOutput = 0.0f;
  } else {
    isCrossFading = true;
    crossFadeIndex = 0;
    nextVelocity = velocity;
  }
}
