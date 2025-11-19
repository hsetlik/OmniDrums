#pragma once
/* This namespace is were all the string identifiers for
 * parameters, components, and anything else should live.
 * Simply use the `DECLARE_ID` macro and never worry about
 * misspelling a string literal again.
 *
 * This translation unit is also responsible for the
 * `ID::getParameterLayout()` function which we use to set up
 * the AudioProcessorValueTreeState that runs everything
 * */
#include <juce_core/juce_core.h>
#include <juce_audio_processors/juce_audio_processors.h>
#include <unordered_map>

// typedefs bc some of these types are a mouthful
typedef juce::AudioProcessorValueTreeState apvts;
typedef std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>
    slider_attach_ptr;
typedef std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment>
    combo_attach_ptr;
typedef juce::NormalisableRange<float> frange_t;
using ValueTree = juce::ValueTree;
using String = juce::String;
using Component = juce::Component;

// little range helper that should be built into JUCE tbh
frange_t rangeWithCenter(float min, float max, float center);
// some defines for things like allowed MIDI notes
#define MIN_MIDI_NUM 35
#define MAX_MIDI_NUM 100
#define NUM_DRUM_CHANNELS 64

// defines for various ranges and defaults
#define CHAN_GAIN_MIN -100.0f
#define CHAN_GAIN_MAX 12.0f
#define CHAN_GAIN_CENTER -12.0f
#define CHAN_GAIN_DEFAULT 0.0f

#define PAN_MIN -100.0f
#define PAN_MAX 100.0f
#define PAN_DEFAULT 0.0f

#define NUM_DRUM_CATEGORIES 9
enum DrumCategE {
  kick,
  snare,
  hiHat,
  tom,
  ride,
  crash,
  clap,
  percussion,
  other
};
const juce::StringArray drumCategoryNames = {"Kick", "Snare",      "Hi-hat",
                                             "Tom",  "Ride",       "Crash",
                                             "Clap", "Percussion", "Other"};
inline String stringFor(const DrumCategE& categ) {
  return drumCategoryNames[(int)categ];
}

// a hash table of general MIDI drum names.
// keys are MIDI note #s and values are the names used in the MIDI
// standard's drum map per this website:
// https://qsrdrums.com/webhelp-responsive/References/r_general_midi_drum_kit.html
const std::unordered_map<int, String>& generalMIDIDrumNames();

#define DECLARE_ID(name) const juce::Identifier name(#name);

namespace ID {
// top level ID for the apvts
DECLARE_ID(OmniDrums_audioState)
DECLARE_ID(OmniDrums_sampleState)

// per-channel audio params
DECLARE_ID(channelActive)
DECLARE_ID(channelPan)
DECLARE_ID(channelGain)
DECLARE_ID(channelMidiNum)

// sample library stuff-------------------------
DECLARE_ID(OmniPlayerSample)    // type for the child tree that represents a
                                // sample as loaded into a drum channel
DECLARE_ID(sampleDrumCategory)  // int representing the category folder we can
                                // find this sample in
DECLARE_ID(sampleFileName)      // string path relative to the category folder
DECLARE_ID(
    sampleDrumChannel)  // index of the drum channel this sample is loaded to

}  // namespace ID

#undef DECLARE_ID
