#include "OmniDrums/OmniState.h"
#include "OmniDrums/Common.h"
#include "OmniDrums/Identifiers.h"
#include "OmniDrums/OmniSampleLibrary.h"

GraphingData::GraphingData() {
  for (size_t c = 0; c < NUM_DRUM_CHANNELS; ++c) {
    leftChanLevels[c] = 0.0f;
    rightChanLevels[c] = 0.0f;
  }
}

void GraphingData::setLeftChannelLevel(int idx, float rmsLevel) {
  leftChanLevels[(size_t)idx].store(rmsLevel);
}
void GraphingData::setRightChannelLevel(int idx, float rmsLevel) {
  rightChanLevels[(size_t)idx].store(rmsLevel);
}

float GraphingData::getLeftChannelLevel(int idx) const {
  return leftChanLevels[(size_t)idx].load();
}
float GraphingData::getRightChannelLevel(int idx) const {
  return rightChanLevels[(size_t)idx].load();
}

void GraphingData::timerCallback() {
  updateWanted = true;
}

void GraphingData::updateFinished() {
  updateWanted = false;
  triggerAsyncUpdate();
}

void GraphingData::handleAsyncUpdate() {
  for (auto* l : listeners)
    l->graphingDataUpdated();
}

void GraphingData::registerListener(Listener* l) {
  listeners.push_back(l);
}

void GraphingData::deregisterListener(Listener* l) {
  for (auto it = listeners.begin(); it != listeners.end(); ++it) {
    if (*it == l) {
      listeners.erase(it);
      return;
    }
  }
}

//=====================================================

static ValueTree defaultSampleState() {
  ValueTree vt(ID::OMNI_SAMPLES_STATE);
  // set up default samples
  for (int i = 0; i < NUM_FACTORY_SAMPLES; ++i) {
    auto sampleState =
        FactorySamples::getDefaultSampleState((FactorySamples::SampleE)i);
    vt.appendChild(sampleState, nullptr);
  }
  return vt;
}

//------------------------------------------------------------------
static std::vector<int> getDefaultActiveChannels() {
  std::vector<int> chans = {};
  auto state = defaultSampleState();
  for (int i = 0; i < state.getNumChildren(); ++i) {
    auto sample = state.getChild(i);
    jassert(sample.hasType(ID::OMNI_PLAYER_SAMPLE));
    const int chan = sample[ID::sampleDrumChannel];
    chans.push_back(chan);
  }
  return chans;
}
static bool isActiveDefault(int channelIdx) {
  static std::vector<int> activeChannels = getDefaultActiveChannels();
  for (auto& c : activeChannels) {
    if (c == channelIdx)
      return true;
  }
  return false;
}

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
    layout.add(buildBoolParam(activeID, activeName, isActiveDefault(i)));
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
    // compressor wet/dry
    auto wetDryID = ID::channelCompressorMix.toString() + iStr;
    auto wetDryName = "Channel " + String(i + 1) + " compressor wet/dry mix";
    auto wetDryRange = rangeWithCenter(WET_DRY_MIN, WET_DRY_MAX, 0.5f);
    layout.add(
        buildFloatParam(wetDryID, wetDryName, wetDryRange, WET_DRY_DEFAULT));
    // midi note
    auto noteID = ID::channelMidiNum.toString() + iStr;
    auto noteName = "Channel " + String(i + 1) + " MIDI note";
    const int defaultNote = MIN_MIDI_NUM + i;
    layout.add(buildIntParam(noteID, noteName, MIN_MIDI_NUM, MAX_MIDI_NUM,
                             defaultNote));
  }

  return layout;
}

//===================================================

OmniState::OmniState(juce::AudioProcessor& proc)
    : audioState(proc, nullptr, ID::OMNI_AUDIO_STATE, getParamLayout()),
      samplesState(defaultSampleState()) {
  manager.registerBasicFormats();
  sampleLib = std::make_unique<OmniSampleLibrary>(&manager);
}

float OmniState::loadAudioParameter(const String& paramID) const {
  return audioState.getRawParameterValue(paramID)->load();
}

bool OmniState::channelHasSample(int channelIdx) const {
  auto sampleTree =
      samplesState.getChildWithProperty(ID::sampleDrumChannel, channelIdx);
  return sampleTree.isValid();
}
