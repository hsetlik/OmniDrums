#include "OmniDrums/Audio/Core/OmniEngine.h"
#include "OmniDrums/Identifiers.h"
#include "juce_audio_basics/juce_audio_basics.h"
//===================================================

static frange_t panRange = rangeWithCenter(PAN_MIN, PAN_MAX, PAN_DEFAULT);
OmniEngine::OmniEngine(OmniState* s)
    : state(s),
      sampleCache(s),
      dryBuffer(2, currentBlockSize),
      compBuffer(2, currentBlockSize) {
  for (int i = 0; i < NUM_DRUM_CHANNELS; ++i) {
    drumChannels.add(new DrumChannel(i, &playingChannels));
  }
}

void OmniEngine::setBlockSize(int newSize) {
  if (newSize != currentBlockSize) {
    dryBuffer.setSize(2, newSize);
    compBuffer.setSize(2, newSize);
    currentBlockSize = newSize;
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
      drumChannels[i]->audioState.pan = panRange.convertTo0to1(panVal);
      drumChannels[i]->audioState.compressorMix = mixVal;
    }
  }
  // 3. update the graphing data if needed
  if (state->graphingData.wantsUpdate()) {
    for (int i = 0; i < NUM_DRUM_CHANNELS; ++i) {
      state->graphingData.setLeftChannelLevel(
          i, drumChannels[i]->leftRMS.getCurrentLevel());
      state->graphingData.setRightChannelLevel(
          i, drumChannels[i]->rightRMS.getCurrentLevel());
    }
    state->graphingData.updateFinished();
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

void OmniEngine::renderChannels(AudioBufF& buf,
                                int startSample,
                                int numSamples) {
  for (auto* chan : drumChannels) {
    chan->renderBlock(buf, startSample, numSamples);
  }
}

void OmniEngine::renderChannels(AudioBufF& dBuf,
                                AudioBufF& cBuf,
                                int startSample,
                                int numSamples) {
  for (auto* chan : drumChannels) {
    chan->renderBlock(dBuf, cBuf, startSample, numSamples);
  }
}

void OmniEngine::renderBlock(juce::AudioBuffer<float>& buffer,
                             juce::MidiBuffer& midiBuf) {
  // 1. update parameters
  prepareForBlock();
  int startSample = 0;
  int numSamples = buffer.getNumSamples();
  jassert(numSamples == currentBlockSize);
  dryBuffer.clear();
  compBuffer.clear();
  state->kbdState.processNextMidiBuffer(midiBuf, 0, numSamples, true);
  auto midiIterator = midiBuf.findNextSamplePosition(0);
  bool firstEvent = true;
  const juce::ScopedLock sl(cs);
  for (; numSamples > 0; ++midiIterator) {
    if (midiIterator == midiBuf.cend()) {
      renderChannels(dryBuffer, compBuffer, startSample, numSamples);
      break;
    }
    const auto metadata = *midiIterator;
    const int samplesToNextMessage = metadata.samplePosition - startSample;
    if (samplesToNextMessage >= numSamples) {
      renderChannels(dryBuffer, compBuffer, startSample, numSamples);
      handleMidiMessage(metadata.getMessage());
      break;
    }
    auto minLength = firstEvent ? 1 : minimunBlockSize;
    if (samplesToNextMessage < minLength) {
      handleMidiMessage(metadata.getMessage());
      continue;
    }
    firstEvent = false;
    renderChannels(dryBuffer, compBuffer, startSample, samplesToNextMessage);
    handleMidiMessage(metadata.getMessage());
    startSample += samplesToNextMessage;
    numSamples -= samplesToNextMessage;
  }

  std::for_each(midiIterator, midiBuf.cend(),
                [&](const juce::MidiMessageMetadata& meta) {
                  handleMidiMessage(meta.getMessage());
                });
  // now copy the dry/ comp buffers into the output
  buffer.addFrom(0, 0, dryBuffer, 0, 0, currentBlockSize);
  buffer.addFrom(1, 0, dryBuffer, 1, 0, currentBlockSize);
  // TODO: manipulate the compressed buffer here eventually,
  //  for now just add it to the output
  buffer.addFrom(0, 0, compBuffer, 0, 0, currentBlockSize);
  buffer.addFrom(1, 0, compBuffer, 1, 0, currentBlockSize);
}
