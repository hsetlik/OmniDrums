#pragma once
#include "../OmniState.h"
#include "OmniDrums/GUI/Shared/Color.h"
#include "OmniDrums/GUI/Shared/Fonts.h"
#include "juce_events/juce_events.h"
#include "juce_gui_basics/juce_gui_basics.h"

class ViewRadioButton : public juce::ToggleButton {
private:
  juce::Path getTextPath(const frect_t fBounds) const;

public:
  const String btnText;
  ViewRadioButton(const String& txt);
  void paintButton(juce::Graphics& g, bool isHighlighted, bool isDown) override;
};

//----------------------------------------------------------------
class LibOpenCloseButton : public juce::ToggleButton {
public:
  LibOpenCloseButton() : juce::ToggleButton("libOpenClose") {
    setToggleState(false, juce::dontSendNotification);
  }

  void paintButton(juce::Graphics& g, bool isHighlighted, bool isDown) override;
};

//----------------------------------------------------------------
enum ViewE { vSamples, vCompressor, vRoom };
class ViewSelectorListener {
public:
  ViewSelectorListener() {}
  virtual ~ViewSelectorListener() {}
  virtual void viewSelected(const ViewE& id) = 0;
  virtual void libStateChanged(bool shoudBeOpen) = 0;
};

//----------------------------------------------------------------

class ViewSelector : public Component {
private:
  OmniState* const state;
  ViewRadioButton sampleBtn;
  ViewRadioButton compressorBtn;
  ViewRadioButton roomBtn;
  LibOpenCloseButton libBtn;
  ViewRadioButton* selectedBtn = &sampleBtn;
  std::vector<ViewSelectorListener*> listeners;

public:
  ViewSelector(OmniState* s);
  void resized() override;
  void paint(juce::Graphics& g) override;
  bool isSelected(ViewRadioButton* btn) const { return btn == selectedBtn; }
  void setSelected(ViewRadioButton* btn);
  void addListener(ViewSelectorListener* ptr);
  void removeListener(ViewSelectorListener* ptr);
};
