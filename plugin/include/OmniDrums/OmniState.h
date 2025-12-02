#pragma once
#include "Identifiers.h"
#include "OmniSampleLibrary.h"
#include "juce_audio_basics/juce_audio_basics.h"
#include "juce_audio_formats/juce_audio_formats.h"
#include "juce_events/juce_events.h"
// this class atomically stores all the data that our
// graphing components (VU meters, compression graphs, etc)
// need to draw themselves
typedef std::atomic<float> float_at;
typedef std::atomic<bool> bool_at;
class GraphingData : public juce::Timer, public juce::AsyncUpdater {
private:
  bool updateWanted = false;
  std::array<float_at, NUM_DRUM_CHANNELS> leftChanLevels;
  std::array<float_at, NUM_DRUM_CHANNELS> rightChanLevels;

public:
  GraphingData();
  // this allows us to skip all the graph updating code
  // when the UI isn't open
  void editorOpened() { startTimerHz(MAX_REPAINT_HZ); }
  void editorClosed() { stopTimer(); }
  void timerCallback() override;
  void handleAsyncUpdate() override;
  bool wantsUpdate() const { return updateWanted; }
  // note: all these getters/setters are atomic
  void setLeftChannelLevel(int idx, float rmsLevel);
  void setRightChannelLevel(int idx, float rmsLevel);
  float getLeftChannelLevel(int idx) const;
  float getRightChannelLevel(int idx) const;
  // call this when we finish updating all the parameters
  // at the beginning of a block
  void updateFinished();

  // graphing components should inherit from this,
  // graphingDataUpdated will be called asynchronously
  // from the message thread so none of our UI components can
  // block the audio thread
  struct Listener {
    Listener() = default;
    virtual ~Listener() {}
    virtual void graphingDataUpdated() = 0;
  };
  void registerListener(Listener* l);
  void deregisterListener(Listener* l);

private:
  std::vector<Listener*> listeners;
};

//=====================================================

class OmniState {
private:
  juce::AudioFormatManager manager;
  int selectedChannel = -1;
  bool libIsOpen = false;

public:
  apvts audioState;
  ValueTree samplesState;
  GraphingData graphingData;
  std::unique_ptr<OmniSampleLibrary> sampleLib;
  juce::MidiKeyboardState kbdState;

  float loadAudioParameter(const String& paramID) const;

  OmniState(juce::AudioProcessor& proc);
  juce::AudioFormatManager* getManager() { return &manager; }
  bool channelHasSample(int channelIdx) const;
  bool channelIsSelected(int idx) const { return idx == selectedChannel; }
  void selectChannel(int idx) { selectedChannel = idx; }
  void setLibOpen(bool shouldBeOpen) { libIsOpen = shouldBeOpen; }
  bool isLibOpen() const { return libIsOpen; }
  float mainViewWidth() const;
};
