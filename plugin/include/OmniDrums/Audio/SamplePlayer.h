#pragma once
#include "../OmniSampleLibrary.h"
#include "juce_events/juce_events.h"

// Sample players should inherit from this to be notified of sample rate changes
class SampleRateListener {
public:
  SampleRateListener() = default;
  virtual ~SampleRateListener() {}
  virtual void sampleRateSet(double sr) = 0;
};

// deals with parsing and converting audio files
// of various file formats, sample rates, channel layouts, etc
namespace AudioFile {
size_t samplesNeededFor(const juce::File& sample, double playbackSampleRate);
double getCurrentSampleRate();
// call this from PluginProcessor.cpp any time the host sets the sample rate
void setSampleRate(double newRate);
juce::AudioFormatReader* getReaderFor(const juce::File& sample);
// listener stuff
void registerListener(SampleRateListener* l);
void deregisterListener(SampleRateListener* l);

}  // namespace AudioFile

class SamplePlaybackBuffer {
public:
  const double playbackSampleRate;
  const size_t lengthInSamples;
  SamplePlaybackBuffer(const juce::File& sample, double sampleRate);
  float operator[](int index) const { return buffer[(size_t)index]; }
  float operator[](size_t index) const { return buffer[index]; }
  float getValue(size_t index) const { return buffer[index]; }

private:
  std::unique_ptr<float[]> buffer;
};

class SamplePlayer : public SampleRateListener, public juce::AsyncUpdater {
private:
  juce::File sampleFile;
  bool bufferPrepared = false;
  bool isPlaying = false;
  size_t currentIdx = 0;
  std::unique_ptr<SamplePlaybackBuffer> buf;

public:
  SamplePlayer(const juce::File& sample = FactorySamples::getSampleFile(
                   FactorySamples::kick,
                   OmniSampleLibrary::getSampleLibFolder()));
  void sampleRateSet(double sr) override;
  void handleAsyncUpdate() override;
  // get the next output value & update this player's internal state accordingly
  float tick();
};
