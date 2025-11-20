#pragma once
#include "../../OmniState.h"
#include "../Shared/Color.h"
#include "../Shared/Fonts.h"
#include "OmniDrums/Identifiers.h"
#include "juce_graphics/juce_graphics.h"

class DrumPadComponent : public Component, public juce::Timer {
private:
  OmniState* const state;
  const int channelIdx;
  bool needsRepaint = false;
  bool shouldDrawActive = false;
  int midiNote = 36;
  DrumCategE currentCateg = DrumCategE::percussion;
  juce::Image iconToDraw;

  void updateIcon();
  void updateMidiNote();

public:
  DrumPadComponent(OmniState* s, int idx);
  void paint(juce::Graphics& g) override;
  void resized() override;
  void timerCallback() override;
  void mouseDown(const juce::MouseEvent& e) override;
  void mouseUp(const juce::MouseEvent& e) override;
};

//============================================================
class SampleNameComponent : public Component, public ValueTree::Listener {
private:
  OmniState* const state;
  const int channelIdx;

public:
  SampleNameComponent(OmniState* s, int idx);
  void paint(juce::Graphics& g) override;
};
//============================================================
class OmniChannelComponent : public Component {
private:
  OmniState* const state;
  const int channelIdx;
  slider_attach_ptr gainAttach;
  juce::Slider gainSlider;
  slider_attach_ptr panAttach;
  juce::Slider panSlider;
  DrumPadComponent drumPad;

public:
  OmniChannelComponent(OmniState* s, int chanIdx);
  // basic component stuff
  void resized() override;
  void paint(juce::Graphics& g) override;
};
