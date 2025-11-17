#pragma once
#include "../OmniState.h"
#include "OmniDrums/GUI/Shared/OmniLookAndFeel.h"

class OmniEditor : public juce::Component {
private:
  OmniState* const state;
  OmniLookAndFeel lnf;

public:
  OmniEditor(OmniState* s);
  ~OmniEditor() override;
  void paint(juce::Graphics& g) override;
};
