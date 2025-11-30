#pragma once
#include "../OmniSampleLibrary.h"
#include "juce_audio_formats/juce_audio_formats.h"
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
size_t samplesNeededFor(juce::AudioFormatManager* manager,
                        const juce::File& sample,
                        double playbackSampleRate);
double getCurrentSampleRate();
// call this from PluginProcessor.cpp any time the host sets the sample rate
void setSampleRate(double newRate);

// listener stuff
void registerListener(SampleRateListener* l);
void deregisterListener(SampleRateListener* l);

}  // namespace AudioFile

class SamplePlaybackBuffer {
public:
  const double playbackSampleRate;
  const size_t lengthInSamples;
  SamplePlaybackBuffer(juce::AudioFormatManager* manager,
                       const juce::File& sample,
                       double sampleRate);
  float operator[](int index) const { return buffer[(size_t)index]; }
  float operator[](size_t index) const { return buffer[index]; }
  float getValue(size_t index) const { return buffer[index]; }

private:
  std::unique_ptr<float[]> buffer;
};

class SamplePlayer : public SampleRateListener {
private:
  juce::AudioFormatManager* const parentManager;
  double prevSampleRate = 44100.0;
  juce::File sampleFile;
  std::unique_ptr<SamplePlaybackBuffer> buf;

public:
  SamplePlayer(juce::AudioFormatManager* manager,
               const juce::File& sample = FactorySamples::getSampleFile(
                   FactorySamples::kick,
                   OmniSampleLibrary::getSampleLibFolder()));
  ~SamplePlayer() override;
  void sampleRateSet(double sr) override;
  size_t lengthInSamples() const { return buf->lengthInSamples; }
  String getSampleName() const { return sampleFile.getFileName(); }
  // access the underlying playback buffer
  float operator[](size_t index) const { return buf->getValue(index); }
  float getValue(size_t index) const { return buf->getValue(index); }
};
