#pragma once
#include "Identifiers.h"
#include "Common.h"
#include "juce_audio_formats/juce_audio_formats.h"

namespace FactorySamples {
enum SampleE { closedHat, openHat, hiTom, midTom, lowTom, ride, snare, kick };
juce::AudioFormatReader* getReader(const SampleE& id);
}  // namespace FactorySamples

class OmniSampleLibrary {
public:
  static juce::File getSampleLibFolder();
};
