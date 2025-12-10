#pragma once
#include "../OmniState.h"
#include "OmniDrums/GUI/Browser/SampleList.h"
#include "OmniDrums/GUI/Shared/OmniLookAndFeel.h"
#include "SampleView/SampleView.h"
#include "ViewSelector.h"
#include "juce_gui_basics/juce_gui_basics.h"

class OmniEditor : public juce::Component,
                   public ViewSelectorListener,
                   public juce::DragAndDropContainer {
private:
  OmniState* const state;
  OmniLookAndFeel lnf;
  SampleView sampleView;
  ViewSelector viewSelector;
  SampleBrowser browser;

public:
  OmniEditor(OmniState* s);
  ~OmniEditor() override;
  void paint(juce::Graphics& g) override;
  void resized() override;
  void viewSelected(const ViewE& id) override;
  void libStateChanged(bool libIsOpen) override;
};
