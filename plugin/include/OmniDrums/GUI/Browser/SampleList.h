#pragma once
#include "../../OmniState.h"
#include "../../Audio/Core/SampleCache.h"
#include "OmniDrums/Identifiers.h"

// component for each sample in the list
class LibEntryComponent : public Component {
  OmniState* const state;
  ValueTree sampleTree;

public:
  LibEntryComponent(OmniState* s, ValueTree tree);
  void paint(juce::Graphics& g) override;
};

