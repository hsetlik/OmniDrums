#pragma once

#include "PluginProcessor.h"

namespace audio_plugin {

class OmniDrumsProcessorEditor : public juce::AudioProcessorEditor {
public:
  explicit OmniDrumsProcessorEditor(OmniDrumsAudioProcessor&);
  ~OmniDrumsProcessorEditor() override;
  void paint(juce::Graphics&) override;
  void resized() override;

private:
  // This reference is provided as a quick way for your editor to
  // access the processor object that created it.
  OmniDrumsAudioProcessor& processorRef;
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OmniDrumsProcessorEditor)
};

}  // namespace audio_plugin
