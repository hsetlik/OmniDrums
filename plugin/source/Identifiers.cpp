#include "OmniDrums/Identifiers.h"

frange_t rangeWithCenter(float min, float max, float center) {
  frange_t range(min, max);
  range.setSkewForCentre(center);
  return range;
}
int channelNumForNote(int midiNote) {
  auto chan = midiNote - MIN_MIDI_NUM;
  jassert(chan >= 0 && chan < NUM_DRUM_CHANNELS);
  return chan;
}

int noteForChannelNum(int channelNum) {
  auto note = channelNum + MIN_MIDI_NUM;
  jassert(note >= MIN_MIDI_NUM && note < MAX_MIDI_NUM);
  return note;
}
