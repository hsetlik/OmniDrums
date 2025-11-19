#pragma once
#include "OmniDrums/Audio/SamplePlayer.h"
#include <cmath>
#include "juce_core/juce_core.h"

namespace AudioFile {
// our state variables
static double currentSampleRate = 44100.0f;
static std::vector<SampleRateListener*> listeners = {};
// formatManager/reader stuff
static juce::AudioFormatManager manager;
static bool managerPrepared = false;

size_t samplesNeededFor(const juce::File& sample, double playbackSampleRate) {
  if (!managerPrepared) {
    manager.registerBasicFormats();
    managerPrepared = true;
  }
  auto reader = manager.createReaderFor(sample);
  jassert(reader != nullptr);
  const double fileDurationSeconds =
      (double)reader->lengthInSamples / reader->sampleRate;
  return (size_t)(fileDurationSeconds * playbackSampleRate);
}

juce::AudioFormatReader* getReaderFor(const juce::File& sample) {
  if (!managerPrepared) {
    manager.registerBasicFormats();
    managerPrepared = true;
  }
  return manager.createReaderFor(sample);
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

SamplePlaybackBuffer::SamplePlaybackBuffer(const juce::File& sample,
                                           double sampleRate)
    : playbackSampleRate(sampleRate),
      lengthInSamples(AudioFile::samplesNeededFor(sample, sampleRate)),
      buffer(new float[lengthInSamples]) {
  auto reader = AudioFile::getReaderFor(sample);
  jassert(reader != nullptr);
  // 1. load the file into an AudioBuffer (this handles sample type conversion
  // for us)
  juce::AudioBuffer<float> fileBuf((int)reader->numChannels,
                                   (int)reader->lengthInSamples);
  jassert(
      reader->read(&fileBuf, 0, (int)reader->lengthInSamples, 0, true, true));
  // 2. scale/convert as needed and write into the buffer
  for (size_t s = 0; s < lengthInSamples; ++s) {
    const float pos = (float)s / (float)lengthInSamples;
    buffer[s] = monoAtRelativePos(fileBuf, pos);
  }
}

//===================================================

SamplePlayer::SamplePlayer(const juce::File& sample)
    : sampleFile(sample),
      buf(std::make_unique<SamplePlaybackBuffer>(
          sampleFile,
          AudioFile::getCurrentSampleRate())) {
  AudioFile::registerListener(this);
}

SamplePlayer::~SamplePlayer() {
  AudioFile::deregisterListener(this);
}

void SamplePlayer::sampleRateSet(double newRate) {
  buf.reset(new SamplePlaybackBuffer(sampleFile, newRate));
}
