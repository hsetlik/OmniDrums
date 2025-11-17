#include "OmniDrums/OmniState.h"
#include "OmniDrums/Common.h"
#include "OmniDrums/Identifiers.h"

// helpers for APVTS components
static std::unique_ptr<juce::AudioParameterFloat> buildFloatParam(
    const String& id,
    const String& name,
    const frange_t& range,
    float defaultVal) {
  juce::ParameterID paramID{id, 1};
  return std::make_unique<juce::AudioParameterFloat>(paramID, name, range,
                                                     defaultVal);
}

static std::unique_ptr<juce::AudioParameterBool>
buildBoolParam(const String& id, const String& name, bool defaultVal) {
  juce::ParameterID paramID{id, 1};
  return std::make_unique<juce::AudioParameterBool>(paramID, name, defaultVal);
}

static std::unique_ptr<juce::AudioParameterInt> buildIntParam(
    const String& id,
    const String& name,
    int min,
    int max,
    int defaultVal) {
  juce::ParameterID paramID{id, 1};
  return std::make_unique<juce::AudioParameterInt>(paramID, name, min, max,
                                                   defaultVal);
}

static apvts::ParameterLayout getParamLayout() {
  apvts::ParameterLayout layout;
  // all the parameters for per-channel gain, pan, pitch shift, etc. go here
  for (int i = 0; i < NUM_DRUM_CHANNELS; ++i) {
    String iStr(i);
    // active
    auto activeID = ID::channelActive.toString() + iStr;
    auto activeName = "Channel " + String(i + 1) + " active";
    layout.add(buildBoolParam(activeID, activeName, false));
    // gain
    auto gainID = ID::channelGain.toString() + iStr;
    auto gainName = "Channel " + String(i + 1) + " gain";
    auto gainRange =
        rangeWithCenter(CHAN_GAIN_MIN, CHAN_GAIN_MAX, CHAN_GAIN_CENTER);
    layout.add(buildFloatParam(gainID, gainName, gainRange, CHAN_GAIN_DEFAULT));
    // pan
    auto panID = ID::channelPan.toString() + iStr;
    auto panName = "Channel " + String(i + 1) + " pan";
    auto panRange = rangeWithCenter(PAN_MIN, PAN_MAX, PAN_DEFAULT);
    layout.add(buildFloatParam(panID, panName, panRange, PAN_DEFAULT));
    // midi note
    auto noteID = ID::channelMidiNum.toString() + iStr;
    auto noteName = "Channel " + String(i + 1) + " MIDI note";
    const int defaultNote = MIN_MIDI_NUM + i;
    layout.add(buildIntParam(noteID, noteName, MIN_MIDI_NUM, MAX_MIDI_NUM,
                             defaultNote));
  }

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
