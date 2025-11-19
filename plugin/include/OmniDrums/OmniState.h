#pragma once

#include "Common.h"
#include "Identifiers.h"
#include "OmniSampleLibrary.h"
class OmniState {
private:
  apvts audioState;
  ValueTree samplesState;
  OmniSampleLibrary sampleLib;

public:
  OmniState(juce::AudioProcessor& proc);
};
