#pragma once
#include "../OmniState.h"
#include "OmniDrums/GUI/Shared/OmniLookAndFeel.h"
#include "SampleView/SampleView.h"
#include "ViewSelector.h"

class OmniEditor : public juce::Component {
private:
  OmniState* const state;
  OmniLookAndFeel lnf;
  SampleView sampleView;
  ViewSelector viewSelector;

public:
  OmniEditor(OmniState* s);
  ~OmniEditor() override;
  void paint(juce::Graphics& g) override;
  void resized() override;
};
