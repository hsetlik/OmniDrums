#include "OmniDrums/PluginEditor.h"
#include "OmniDrums/PluginProcessor.h"
#include "juce_core/juce_core.h"

namespace audio_plugin {
OmniDrumsProcessorEditor::OmniDrumsProcessorEditor(OmniDrumsAudioProcessor& p)
    : AudioProcessorEditor(&p), editor(&p.state), processorRef(p) {
  juce::ignoreUnused(processorRef);
  // Make sure that before the constructor has finished, you've set the
  // editor's size to whatever you need it to be.
  setSize(1800, 1080);
  addAndMakeVisible(editor);
}

OmniDrumsProcessorEditor::~OmniDrumsProcessorEditor() {}

void OmniDrumsProcessorEditor::paint(juce::Graphics& g) {
  juce::ignoreUnused(g);
}

void OmniDrumsProcessorEditor::resized() {
  editor.setBounds(getLocalBounds());
}
}  // namespace audio_plugin
