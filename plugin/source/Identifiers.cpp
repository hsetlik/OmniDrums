#include "OmniDrums/Identifiers.h"

frange_t rangeWithCenter(float min, float max, float center) {
  frange_t range(min, max);
  range.setSkewForCentre(center);
  return range;
}

static std::unordered_map<int, String> createDrumNameMap() {
  std::unordered_map<int, String> map = {{35, "Acoustic Bass Drum"},
                                         {36, "Bass Drum"},
                                         {37, "Side Stick"},
                                         {38, "Acoustic Snare"},
                                         {39, "Hand Clap"},
                                         {40, "Electric Snare"},
                                         {41, "Low Floor Tom"},
                                         {42, "Closed Hi Hat"},
                                         {43, "High Floor Tom"},
                                         {44, "Pedal Hi Hat"},
                                         {45, "Low Tom"},
                                         {46, "Open Hi-Hat"},
                                         {47, "Low-Mid Tom"},
                                         {48, "Hi-Mid Tom"},
                                         {49, "Crash Cymbal 1"},
                                         {50, "High Tom"},
                                         {51, "Ride Cymbal 1"},
                                         {52, "Chinese Cymbal"},
                                         {53, "Ride Bell"},
                                         {54, "Tabourine"},
                                         {55, "Splash Cymbal"},
                                         {56, "Cowbell"},
                                         {57, "Crash Cymbal 2"},
                                         {58, "Vibraslap"},
                                         {59, "Ride Cymbal 2"},
                                         {60, "Hi Bongo"},
                                         {61, "Low Bongo"},
                                         {62, "Mute Hi Conga"},
                                         {63, "Open Hi Conga"},
                                         {64, "Low Conga"},
                                         {65, "Hi Timbale"},
                                         {66, "Low Timbale"},
                                         {67, "Hi Agogo"},
                                         {68, "Low Agogo"},
                                         {69, "Cabasa"},
                                         {70, "Maracas"},
                                         {71, "Short Whistle"},
                                         {72, "Long Whistle"},
                                         {73, "Short Guiro"},
                                         {74, "Long Guiro"},
                                         {75, "Claves"},
                                         {76, "Hi Wood Block"},
                                         {77, "Low Wood Block"},
                                         {78, "Mute Cuica"},
                                         {79, "Open Cuica"},
                                         {80, "Mute Triangle"},
                                         {81, "Open Triangle"}};
  return map;
}

const std::unordered_map<int, String>& generalMIDIDrumNames() {
  static std::unordered_map<int, String> map = createDrumNameMap();
  return map;
}

static std::vector<String> createNoteNameList() {
  std::vector<String> list = {
      "C-2",  "C#-2", "D-2",  "D#-2", "E-2",  "F-2", "F#-2", "G-2", "G#-2",
      "A-2",  "A#-2", "B-2",  "C-1",  "C#-1", "D-1", "D#-1", "E-1", "F-1",
      "F#-1", "G-1",  "G#-1", "A-1",  "A#-1", "B-1", "C0",   "C#0", "D0",
      "D#0",  "E0",   "F0",   "F#0",  "G0",   "G#0", "A0",   "A#0", "B0",
      "C1",   "C#1",  "D1",   "D#1",  "E1",   "F1",  "F#1",  "G1",  "G#1",
      "A1",   "A#1",  "B1",   "C2",   "C#2",  "D2",  "D#2",  "E2",  "F2",
      "F#2",  "G2",   "G#2",  "A2",   "A#2",  "B2",  "C3",   "C#3", "D3",
      "D#3",  "E3",   "F3",   "F#3",  "G3",   "G#3", "A3",   "A#3", "B3",
      "C4",   "C#4",  "D4",   "D#4",  "E4",   "F4",  "F#4",  "G4",  "G#4",
      "A4",   "A#4",  "B4",   "C5",   "C#5",  "D5",  "D#5",  "E5",  "F5",
      "F#5",  "G5",   "G#5",  "A5",   "A#5",  "B5",  "C6",   "C#6", "D6",
      "D#6",  "E6",   "F6",   "F#6",  "G6",   "G#6", "A6",   "A#6", "B6",
      "C7",   "C#7",  "D7",   "D#7",  "E7",   "F7",  "F#7",  "G7",  "G#7",
      "A7",   "A#7",  "B7",   "C8",   "C#8",  "D8",  "D#8",  "E8",  "F8",
      "F#8",  "G8",
  };
  return list;
}

String getMIDINoteName(int note) {
  static std::vector<String> noteNames = createNoteNameList();
  jassert(note < 128);
  return noteNames[(size_t)note];
}

String getMIDIDrumName(int midiNote) {
  static std::unordered_map<int, String> map = generalMIDIDrumNames();
  if (map.contains(midiNote)) {
    return map[midiNote];
  }
  return "Null";
}
