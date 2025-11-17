#pragma once

#include "Common.h"
#include "Identifiers.h"

class OmniState {
private:
  apvts audioState;
  ValueTree samplesState;

public:
  OmniState(juce::AudioProcessor& proc);
};
