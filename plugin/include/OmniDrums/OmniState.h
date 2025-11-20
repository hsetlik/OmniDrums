#pragma once

#include "Common.h"
#include "Identifiers.h"
#include "OmniSampleLibrary.h"

class OmniState {
public:
  apvts audioState;
  ValueTree samplesState;
  OmniSampleLibrary sampleLib;
  float loadAudioParameter(const String& paramID) const;

  OmniState(juce::AudioProcessor& proc);
};
