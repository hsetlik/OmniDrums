#include "OmniDrums/Audio/Core/DrumChannel.h"

#define CROSS_FADE_SAMPLES 100
//===================================================

PlayingChannelSet::PlayingChannelSet() : playingChannels({}) {}

void PlayingChannelSet::channelStarted(int channel) {
  playingChannels.push_back(channel);
}

void PlayingChannelSet::channelFinished(int channel) {
  for (auto it = playingChannels.begin(); it != playingChannels.end(); ++it) {
    if (*it == channel) {
      playingChannels.erase(it);
      return;
    }
  }
}

//===================================================
DrumChannel::DrumChannel(int c, PlayingChannelSet* p)
    : channel(c), playingSet(p) {}

void DrumChannel::tick() {
  if (!isPlaying)
    return;
  if (!isCrossFading) {
    lastOutput = player->getValue(currentIndex);
    ++currentIndex;
    if (currentIndex >= player->lengthInSamples()) {
      isPlaying = false;
      playingSet->channelFinished(channel);
    }
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
    playingSet->channelStarted(channel);
  } else {
    isCrossFading = true;
    crossFadeIndex = 0;
    nextVelocity = velocity;
  }
}

void DrumChannel::renderSamplesDryMix(float& left, float& right) const {
  left += lastOutput * audioState.pan * audioState.gainLinear *
          (1.0f - audioState.compressorMix);
  right += lastOutput * (1.0f - audioState.pan) * audioState.gainLinear *
           (1.0f - audioState.compressorMix);
}

void DrumChannel::renderSamplesCompressorMix(float& left, float& right) const {
  left += lastOutput * audioState.pan * audioState.gainLinear *
          audioState.compressorMix;
  right += lastOutput * (1.0f - audioState.pan) * audioState.gainLinear * 1.0f -
           audioState.compressorMix;
}
