//===================================================
#include "OmniDrums/Audio/Core/SampleCache.h"
#include "OmniDrums/Identifiers.h"

SampleCache::SampleCache(OmniState* s) : state(s) {
  if (state->samplesState.getNumChildren() > 0) {
    for (int c = 0; c < state->samplesState.getNumChildren(); ++c) {
      auto child = state->samplesState.getChild(c);
      jassert(child.hasType(ID::OMNI_PLAYER_SAMPLE));
      addPlayerForTree(child);
    }
  }
  state->samplesState.addListener(this);
}

SampleCache::~SampleCache() {
  state->samplesState.removeListener(this);
}

void SampleCache::addPlayerForTree(ValueTree& child) {
  auto file = state->sampleLib.fileForSample(child);
  const int channelToNotify = child[ID::sampleDrumChannel];
  auto* player = players.add(new SamplePlayer(file));
  if (channelPlayers[(size_t)channelToNotify] != nullptr) {
    readyToFree.push_back(channelPlayers[(size_t)channelToNotify]);
  }
  channelPlayers[(size_t)channelToNotify] = player;
}

SamplePlayer* SampleCache::playerWithName(const String& name) {
  for (int i = 0; i < players.size(); ++i) {
    if (players[i]->getSampleName() == name) {
      return players[i];
    }
  }
  return nullptr;
}

void SampleCache::prepareToFreePlayer(const String& sampleName) {
  auto* ptr = playerWithName(sampleName);
  if (ptr != nullptr) {
    readyToFree.push_back(ptr);
  }
}

bool SampleCache::isQueuedToFree(SamplePlayer* player) const {
  if (readyToFree.size() > 0 && player != nullptr) {
    for (auto* p : readyToFree) {
      if (p == player)
        return true;
    }
  }
  return false;
}

void SampleCache::cleanupSampleData() {
  if (readyToFree.size() > 0) {
    for (auto* p : readyToFree) {
      players.removeObject(p);
    }
    readyToFree.clear();
  }
}

void SampleCache::valueTreeChildAdded(ValueTree& parent, ValueTree& newChild) {
  jassert(parent.hasType(ID::OMNI_SAMPLES_STATE) &&
          newChild.hasType(ID::OMNI_PLAYER_SAMPLE));
  addPlayerForTree(newChild);
}

void SampleCache::valueTreeChildRemoved(ValueTree& parent,
                                        ValueTree& removedChild,
                                        int index) {
  juce::ignoreUnused(index);
  jassert(parent.hasType(ID::OMNI_SAMPLES_STATE) &&
          removedChild.hasType(ID::OMNI_PLAYER_SAMPLE));
  const String name = removedChild[ID::sampleFileName];
  auto* player = playerWithName(name);
  if (!isQueuedToFree(player) && player != nullptr) {
    readyToFree.push_back(player);
  }
}
