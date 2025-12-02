#include "OmniDrums/GUI/SampleView/SampleView.h"
#include "OmniDrums/GUI/SampleView/ChannelComponent.h"
#include "OmniDrums/GUI/Shared/Util.h"
//===================================================

ViewedChannelsComponent::ViewedChannelsComponent(OmniState* s) : state(s) {
  // initialize all the channels here
  for (int i = 0; i < NUM_DRUM_CHANNELS; ++i) {
    channels.add(new OmniChannelComponent(s, i));
    addAndMakeVisible(channels.getLast());
  }
}

void ViewedChannelsComponent::resized() {
  auto editorParent = findParentComponentOfClass<SampleView>();
  jassert(editorParent != nullptr);
  auto parentBounds = editorParent->getBounds().toFloat();
  const float xScale = parentBounds.getWidth() / state->mainViewWidth();
  const float yScale = parentBounds.getHeight() / 630.0f;
  static const float chanHeight = 210.0f;
  static const float chanWidth = 296.0f;
  static const int colHeight = 3;
  int numCols = 0;
  for (int c = 0; c < channels.size(); ++c) {
    int row = c % colHeight;
    frect_t chanBounds = {(float)numCols * chanWidth, (float)row * chanHeight,
                          chanWidth, chanHeight};
    channels[c]->setBounds(
        GraphicsUtil::scaledFor(chanBounds, xScale, yScale).toNearestInt());
    if (row == colHeight - 1) {
      ++numCols;
    }
  }
  auto totalWidth = (float)numCols * chanWidth * xScale;
  auto totalHeight = (float)colHeight * chanHeight * yScale;
  setSize((int)totalWidth, (int)totalHeight);
}

//===================================================

SampleView::SampleView(OmniState* s) : channelComp(s) {
  vpt.setViewedComponent(&channelComp, false);
  vpt.setViewPosition(0, 0);
  vpt.setInterceptsMouseClicks(true, true);
  vpt.setScrollBarsShown(false, true, false, false);
  addAndMakeVisible(vpt);
  channelComp.resized();
}

void SampleView::resized() {
  channelComp.resized();
  vpt.setBounds(getLocalBounds());
}
