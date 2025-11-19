#pragma once

#include "Common.h"
#include "Identifiers.h"
#include "OmniSampleLibrary.h"
class OmniState {
public:
  apvts audioState;
  ValueTree samplesState;
  OmniSampleLibrary sampleLib;

  OmniState(juce::AudioProcessor& proc);
};
