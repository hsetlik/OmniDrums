#include "OmniDrums/OmniState.h"
#include "OmniDrums/Identifiers.h"

static apvts::ParameterLayout getParamLayout() {
  apvts::ParameterLayout layout;
  // all the parameters for per-channel gain, pan, pitch shift, etc. go here

  return layout;
}

static ValueTree defaultSampleState() {
  ValueTree vt(ID::OmniDrums_sampleState);
  // set up default samples

  return vt;
}
//===================================================
OmniState::OmniState(juce::AudioProcessor& proc)
    : audioState(proc, nullptr, ID::OmniDrums_audioState, getParamLayout()),
      samplesState(defaultSampleState()) {}
