#pragma once
#include "SearchHeader.h"

// component for each sample in the list
class LibEntryComponent : public Component {
  OmniState* const state;
  ValueTree sampleTree;

public:
  LibEntryComponent(OmniState* s, ValueTree tree);
  void paint(juce::Graphics& g) override;
};

//===========================================

class SampleBrowser : public Component, public SearchHeader::Listener {
private:
  OmniState* const state;
  SearchHeader header;

public:
  SampleBrowser(OmniState* s);
  ~SampleBrowser() override;
  void searchStateChanged(const LibSearchState& searchState) override;
  void resized() override;
};
