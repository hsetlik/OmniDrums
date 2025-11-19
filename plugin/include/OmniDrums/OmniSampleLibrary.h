#pragma once
#include "Identifiers.h"
#include "Common.h"
#include "juce_audio_formats/juce_audio_formats.h"
#define NUM_FACTORY_SAMPLES 8
namespace FactorySamples {
enum SampleE { closedHat, openHat, hiTom, midTom, lowTom, ride, snare, kick };

juce::File getSampleFile(const SampleE& id, const juce::File& libFolder);
ValueTree getDefaultSampleState(const SampleE& id);
}  // namespace FactorySamples

class OmniSampleLibrary {
private:
  juce::File libFolder;

public:
  static juce::File getSampleLibFolder();
  OmniSampleLibrary();
};
