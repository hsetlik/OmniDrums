#pragma once

#include "../../OmniState.h"
#include "OmniDrums/Audio/SamplePlayer.h"
#include "OmniDrums/Identifiers.h"
#include "juce_core/juce_core.h"

class SampleCache : public ValueTree::Listener {
private:
  OmniState* const state;
  juce::OwnedArray<SamplePlayer> players;
  std::vector<SamplePlayer*> readyToFree = {};
  SamplePlayer* channelPlayers[NUM_DRUM_CHANNELS] = {nullptr};

public:
  SampleCache(OmniState* s);
  ~SampleCache() override;
  void valueTreeChildAdded(ValueTree& parent, ValueTree& newChild) override;
  void valueTreeChildRemoved(ValueTree& parent,
                             ValueTree& removedChild,
                             int index) override;
  void valueTreePropertyChanged(ValueTree& changedTree,
                                const juce::Identifier& propertyID) override;
  // each drum channel should set an internal pointer with this
  // function at the beginning of each processBlock
  SamplePlayer* playerForChannel(int drumChannel) const {
    return channelPlayers[(size_t)drumChannel];
  }
  // call this at the end of each processBlock to
  // clean up memory from unused samples
  void cleanupSampleData();

private:
  SamplePlayer* playerWithName(const String& name);
  void addPlayerForTree(ValueTree& child);
  void prepareToFreePlayer(const String& fileName);
  bool isQueuedToFree(SamplePlayer* player) const;
};
