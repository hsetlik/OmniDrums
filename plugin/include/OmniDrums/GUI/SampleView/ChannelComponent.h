#pragma once
#include "../../OmniState.h"
#include "../Shared/Color.h"
#include "../Shared/Fonts.h"
#include "OmniDrums/Identifiers.h"
#include "juce_graphics/juce_graphics.h"

class OmniChannelComponent : public Component {
private:
  OmniState* const state;
  const int channelIdx;
  slider_attach_ptr gainAttach;
  juce::Slider gainSlider;
  slider_attach_ptr panAttach;

public:
  OmniChannelComponent(OmniState* s, int chanIdx);
  // basic component stuff
  void resized() override;
  void paint(juce::Graphics& g) override;
};

