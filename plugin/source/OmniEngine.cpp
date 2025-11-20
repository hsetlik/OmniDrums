#include "OmniDrums/Audio/Core/OmniEngine.h"
#include "OmniDrums/Identifiers.h"
#include "juce_audio_basics/juce_audio_basics.h"
//===================================================

OmniEngine::OmniEngine(OmniState* s) : state(s), sampleCache(s) {
  for (int i = 0; i < NUM_DRUM_CHANNELS; ++i) {
    drumChannels.add(new DrumChannel(i, &playingChannels));
  }
}

void OmniEngine::prepareForBlock() {
  // clear the note map
  std::fill(noteMap.begin(), noteMap.end(), -1);
  for (int i = 0; i < NUM_DRUM_CHANNELS; ++i) {
    // 1. update the player pointers if anything has changed
    drumChannels[i]->setPlayer(sampleCache.playerForChannel(i));
    // 2. if we're using this channel, grip the APVTS values here
    if (drumChannels[i]->inUse()) {
      const float gainDb =
          state->loadAudioParameter(ID::channelGain.toString() + String(i));
      const float panVal =
          state->loadAudioParameter(ID::channelPan.toString() + String(i));
      const float mixVal = state->loadAudioParameter(
          ID::channelCompressorMix.toString() + String(i));
      const float fMidiNote =
          state->loadAudioParameter(ID::channelMidiNum.toString() + String(i));
      noteMap[(size_t)fMidiNote] = i;
      drumChannels[i]->audioState.gainLinear =
          juce::Decibels::decibelsToGain(gainDb);
      drumChannels[i]->audioState.pan = panVal;
      drumChannels[i]->audioState.compressorMix = mixVal;
    }
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
void OmniEngine::handleMidiMessage(const juce::MidiMessage& msg) {
  if (msg.isNoteOn()) {
    size_t noteNum = (size_t)msg.getNoteNumber();
    if (noteMap[noteNum] != -1) {
      drumChannels[noteMap[noteNum]]->trigger(msg.getFloatVelocity());
    }
  }
}

void OmniEngine::renderBlock(juce::AudioBuffer<float>& buffer,
                             juce::MidiBuffer& midiBuf) {
  // 1. update parameters
  prepareForBlock();
  // 2. fill the MIDI queue
  loadMidiMessages(midiBuf, buffer.getNumSamples());

  // 3. iterate through the buffer
  float lDry = 0.0f;
  float rDry = 0.0f;
  float lComp = 0.0f;
  float rComp = 0.0f;
  auto lBuf = buffer.getWritePointer(0);
  auto rBuf = buffer.getWritePointer(1);
  for (int s = 0; s < buffer.getNumSamples(); ++s) {
    // before touching the channels, check if it's time to handle the next
    // MIDI message
    if (midiQueue.front().sampleIdx == s) {
      handleMidiMessage(midiQueue.front().message);
      midiQueue.pop();
    }
    for (int c = 0; c < playingChannels.numPlayingChannels(); ++c) {
      auto chanIdx = playingChannels[c];
      drumChannels[chanIdx]->tick();
      drumChannels[chanIdx]->renderSamplesDryMix(lDry, rDry);
      drumChannels[chanIdx]->renderSamplesCompressorMix(lComp, rComp);
    }
    // TODO: the compressor work happens here, for now we'll
    //  just write the sum of both samples to the output
    lBuf[s] = lDry + lComp;
    rBuf[s] = rDry + rComp;
  }
  // make sure we handled every midi event
  jassert(midiQueue.empty());
}
