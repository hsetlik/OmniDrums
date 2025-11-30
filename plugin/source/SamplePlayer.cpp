#pragma once
#include "OmniDrums/Audio/SamplePlayer.h"
#include <cmath>
#include "juce_audio_basics/juce_audio_basics.h"
#include "juce_audio_formats/juce_audio_formats.h"
#include "juce_core/juce_core.h"

namespace AudioFile {
// our state variables
static double currentSampleRate = 44100.0f;
static std::vector<SampleRateListener*> listeners = {};

size_t samplesNeededFor(juce::AudioFormatManager* manager,
                        const juce::File& sample,
                        double playbackSampleRate) {
  auto reader = juce::rawToUniquePtr(manager->createReaderFor(sample));
  jassert(reader != nullptr);
  const double fileDurationSeconds =
      (double)reader->lengthInSamples / reader->sampleRate;
  return (size_t)(fileDurationSeconds * playbackSampleRate);
}

double getCurrentSampleRate() {
  return currentSampleRate;
}

// call this from PluginProcessor.cpp any time the host sets the sample rate
void setSampleRate(double newRate) {
  currentSampleRate = newRate;
  for (auto* l : listeners) {
    l->sampleRateSet(currentSampleRate);
  }
}
// listener stuff
void registerListener(SampleRateListener* l) {
  listeners.push_back(l);
}
void deregisterListener(SampleRateListener* l) {
  for (auto it = listeners.begin(); it != listeners.end(); ++it) {
    if (*it == l) {
      listeners.erase(it);
      return;
    }
  }
}

}  // namespace AudioFile
//===================================================
// helpers for scaling/converting sample buffers
static float monoAtRelativePos_mono(const juce::AudioBuffer<float>& buf,
                                    float pos) {
  const float fIndex = pos * (float)buf.getNumSamples();
  const int lowIndex = (int)std::floorf(fIndex);
  const int highIndex = (lowIndex + 1) % buf.getNumSamples();
  const float ratio = fIndex - (float)lowIndex;
  return flerp(buf.getSample(0, lowIndex), buf.getSample(0, highIndex), ratio);
}

static float monoAtRelativePos_stereo(const juce::AudioBuffer<float>& buf,
                                      float pos) {
  const float fIndex = pos * (float)buf.getNumSamples();
  const int lowIndex = (int)std::floorf(fIndex);
  const int highIndex = (lowIndex + 1) % buf.getNumSamples();
  const float ratio = fIndex - (float)lowIndex;
  const float lowSample =
      (buf.getSample(0, lowIndex) + buf.getSample(1, lowIndex)) / 2.0f;
  const float highSample =
      (buf.getSample(0, highIndex) + buf.getSample(1, highIndex)) / 2.0f;
  return flerp(lowSample, highSample, ratio);
}

static float monoAtRelativePos(const juce::AudioBuffer<float>& buf, float pos) {
  if (buf.getNumChannels() > 1) {
    return monoAtRelativePos_stereo(buf, pos);
  }
  return monoAtRelativePos_mono(buf, pos);
}

static std::array<float, FADE_OUT_SAMPLES> createGainRamp() {
  std::array<float, FADE_OUT_SAMPLES> arr;
  float gain = 1.0f;
  const float dGain = gain / (float)FADE_OUT_SAMPLES;
  for (size_t i = 0; i < FADE_OUT_SAMPLES; ++i) {
    arr[i] = gain;
    gain -= dGain;
  }
  return arr;
}

static std::array<float, FADE_OUT_SAMPLES> gainRamp = createGainRamp();

SamplePlaybackBuffer::SamplePlaybackBuffer(juce::AudioFormatManager* manager,
                                           const juce::File& sample,
                                           double sampleRate)
    : playbackSampleRate(sampleRate),
      lengthInSamples(AudioFile::samplesNeededFor(manager, sample, sampleRate)),
      buffer(new float[lengthInSamples]) {
  auto reader = juce::rawToUniquePtr(manager->createReaderFor(sample));
  jassert(reader != nullptr);
  // 1. load the file into an AudioBuffer (this handles sample type conversion
  // for us)
  juce::AudioBuffer<float> fileBuf((int)reader->numChannels,
                                   (int)reader->lengthInSamples + 4);
  jassert(reader->read(&fileBuf, 0, (int)reader->lengthInSamples + 4, 0, true,
                       true));

  // 2. scale/convert as needed and write into the buffer
  const size_t fadeOutStart = lengthInSamples - FADE_OUT_SAMPLES;
  float sampleSum = 0.0f;
  for (size_t s = 0; s < lengthInSamples; ++s) {
    const float pos = (float)s / (float)lengthInSamples;
    buffer[s] = monoAtRelativePos(fileBuf, pos);
    if (s >= fadeOutStart) {
      buffer[s] *= gainRamp[s - fadeOutStart];
    }
    sampleSum += buffer[s];
  }
#ifdef REMOVE_DC_BIAS
  const float bias = sampleSum / (float)lengthInSamples;
  const float minBias = (float)juce::Decibels::decibelsToGain(-60.0f);
  if (std::fabs(bias) >= minBias) {
    for (size_t s = 0; s < lengthInSamples; ++s) {
      buffer[s] -= bias;
    }
  }
#endif
}

//===================================================
//
SamplePlayer::SamplePlayer(juce::AudioFormatManager* manager,
                           const juce::File& sample)
    : parentManager(manager),
      sampleFile(sample),
      buf(std::make_unique<SamplePlaybackBuffer>(
          manager,
          sampleFile,
          AudioFile::getCurrentSampleRate())) {
  AudioFile::registerListener(this);
}

SamplePlayer::~SamplePlayer() {
  AudioFile::deregisterListener(this);
}

void SamplePlayer::sampleRateSet(double newRate) {
  if (std::abs(newRate - prevSampleRate) > 1.0) {
    buf.reset(new SamplePlaybackBuffer(parentManager, sampleFile, newRate));
    prevSampleRate = newRate;
  }
}
