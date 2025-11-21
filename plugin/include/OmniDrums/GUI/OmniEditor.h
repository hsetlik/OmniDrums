#pragma once
#include "../OmniState.h"
#include "OmniDrums/GUI/Shared/OmniLookAndFeel.h"
#include "SampleView/SampleView.h"

class OmniEditor : public juce::Component {
private:
  OmniState* const state;
  OmniLookAndFeel lnf;
  SampleView sampleView;

public:
  OmniEditor(OmniState* s);
  ~OmniEditor() override;
  void paint(juce::Graphics& g) override;
  void resized() override;
};
