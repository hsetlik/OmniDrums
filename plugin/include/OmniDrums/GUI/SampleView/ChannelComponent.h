#pragma once
#include "../../OmniState.h"
#include "../Shared/Color.h"
#include "../Shared/Fonts.h"
#include "OmniDrums/Identifiers.h"
#include "juce_graphics/juce_graphics.h"
#include "VUMeter.h"

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
  String getLabelText() const;

public:
  SampleNameComponent(OmniState* s, int idx);
  ~SampleNameComponent() override;
  void paint(juce::Graphics& g) override;
};

//============================================================
class MIDINoteSelector : public Component, public juce::ComboBox::Listener {
private:
  OmniState* const state;
  const int channelIdx;
  param_attach_ptr attach;
  juce::ComboBox cb;

public:
  MIDINoteSelector(OmniState* s, int idx);
  ~MIDINoteSelector() override;
  void resized() override;
  void comboBoxChanged(juce::ComboBox* box) override;
};

//============================================================
class OmniChannelComponent : public Component, public juce::DragAndDropTarget {
private:
  OmniState* const state;
  const int channelIdx;
  juce::Slider gainSlider;
  slider_attach_ptr gainAttach;
  juce::Slider panSlider;
  slider_attach_ptr panAttach;
  DrumPadComponent drumPad;
  SampleNameComponent nameComp;
  ChannelVUMeter vuMeter;
  MIDINoteSelector noteSelector;
  bool leftBtnDown = false;

public:
  OmniChannelComponent(OmniState* s, int chanIdx);
  // basic component stuff
  void resized() override;
  void paint(juce::Graphics& g) override;
  void mouseUp(const juce::MouseEvent& e) override;
  void mouseDown(const juce::MouseEvent& e) override;
  bool isInterestedInDragSource(
      const juce::DragAndDropTarget::SourceDetails& dragSourceDetails) override;
  void itemDropped(
      const juce::DragAndDropTarget::SourceDetails& dragSourceDetails) override;
  void itemDragEnter(const juce::DragAndDropTarget::SourceDetails&) override {}
  void itemDragExit(const juce::DragAndDropTarget::SourceDetails&) override {}
  void itemDragMove(const juce::DragAndDropTarget::SourceDetails&) override {}
};
