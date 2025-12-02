#pragma once
#include "ChannelComponent.h"

// this is the container for all ChannelComponets
// that will ultimately be viewed in a juce::Viewport

class ViewedChannelsComponent : public Component {
private:
  OmniState* const state;
  juce::OwnedArray<OmniChannelComponent> channels;

public:
  ViewedChannelsComponent(OmniState* s);
  void resized() override;
};

class SampleView : public Component {
  ViewedChannelsComponent channelComp;
  juce::Viewport vpt;

public:
  SampleView(OmniState* s);
  void resized() override;
};
