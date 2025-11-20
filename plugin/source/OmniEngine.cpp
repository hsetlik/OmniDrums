#include "OmniDrums/Audio/Core/OmniEngine.h"
#include "OmniDrums/Identifiers.h"
//===================================================

OmniEngine::OmniEngine(OmniState* s) : state(s), sampleCache(s) {
  for (int i = 0; i < NUM_DRUM_CHANNELS; ++i) {
    drumChannels.add(new DrumChannel(i));
  }
}

void OmniEngine::prepareForBlock() {
  // TODO: eventually update gain, pan, etc from the APVTS

  // 2. update the player pointers if anything has changed
  for (int i = 0; i < NUM_DRUM_CHANNELS; ++i) {
    drumChannels[i]->setPlayer(sampleCache.playerForChannel(i));
  }
}

void OmniEngine::loadMidiMessages(juce::MidiBuffer& midi, int maxSample) {
  for (auto it = midi.begin(); it != midi.end(); ++it) {
    auto metadata = *it;
    timed_midi_msg m;
    m.sampleIdx = metadata.samplePosition;
    jassert(m.sampleIdx >= 0 && m.sampleIdx < maxSample);
    m.message = metadata.getMessage();
    midiQueue.push(m);
  }
}
