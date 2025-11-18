#pragma once
#include "../OmniSampleLibrary.h"

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

private:
  std::unique_ptr<float[]> buffer;
};
