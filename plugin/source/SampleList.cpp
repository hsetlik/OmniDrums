#include "OmniDrums/GUI/Browser/SampleList.h"

SampleBrowser::SampleBrowser(OmniState* s) : state(s) {
  addAndMakeVisible(header);
  header.addListener(this);
}

SampleBrowser::~SampleBrowser() {
  header.removeListener(this);
}

void SampleBrowser::searchStateChanged(const LibSearchState& searchState) {
  juce::ignoreUnused(searchState);
}

void SampleBrowser::resized() {
  auto fBounds = getLocalBounds().toFloat();
  const float yScale = fBounds.getHeight() / 610.0f;
  auto headerBounds = fBounds.removeFromTop(48.0f * yScale);
  header.setBounds(headerBounds.toNearestInt());
}
//===================================================
