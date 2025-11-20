#include "OmniDrums/GUI/SampleView/ChannelComponent.h"
#include "OmniDrums/GUI/Shared/Color.h"
#include "OmniDrums/GUI/Shared/Images.h"
#include "OmniDrums/Identifiers.h"

DrumPadComponent::DrumPadComponent(OmniState* s, int idx)
    : state(s),
      channelIdx(idx),
      iconToDraw(
          Icons::getDrumIconWithColor(currentCateg, UIColor::orangeLight)) {
  updateIcon();
  updateMidiNote();
  startTimerHz(MAX_REPAINT_HZ);
}

// returns the category ID of this drum channel, or -1 if the channel is not
// in use.
static int categForDrumChannel(const ValueTree& parent, int drumChannel) {
  for (int c = 0; c < parent.getNumChildren(); ++c) {
    auto child = parent.getChild(c);
    int chan = child[ID::sampleDrumChannel];
    if (chan == drumChannel) {
      int categID = child[ID::sampleDrumCategory];
      return categID;
    }
  }
  return -1;
}

void DrumPadComponent::updateIcon() {
  auto categID = categForDrumChannel(state->samplesState, channelIdx);
  if (categID != -1 && categID != (int)currentCateg) {
    currentCateg = (DrumCategE)categID;
    iconToDraw =
        Icons::getDrumIconWithColor(currentCateg, UIColor::orangeLight);
    needsRepaint = true;
  }
}

void DrumPadComponent::updateMidiNote() {
  const String noteId = ID::channelMidiNum.toString() + String(channelIdx);
  midiNote = (int)state->loadAudioParameter(noteId);
}

void DrumPadComponent::timerCallback() {
  // 1. check if we should draw the drum pad as active or not
  const bool isActiveNow = state->kbdState.isNoteOn(1, midiNote);
  if (isActiveNow != shouldDrawActive) {
    shouldDrawActive = isActiveNow;
    needsRepaint = true;
  }
  // 2. call for a repaint if necessary
  updateIcon();
  updateMidiNote();
  if (needsRepaint) {
    repaint();
  }
}

void DrumPadComponent::paint(juce::Graphics& g) {
  auto fBounds = getLocalBounds().toFloat();
  auto bkgndImg = Assets::getImage(shouldDrawActive ? Assets::DrumPadOn
                                                    : Assets::DrumPadOff);
  g.drawImage(bkgndImg, fBounds);
  auto iconBounds = iconToDraw.getBounds().toFloat();
  const float aspectRatio = iconBounds.getWidth() / iconBounds.getHeight();
  const float iconWidth = fBounds.getWidth() * 0.65f;
  frect_t iconDest = {0.0f, 0.0f, iconWidth, iconWidth / aspectRatio};
  iconDest = iconDest.withCentre(fBounds.getCentre());
  g.drawImage(iconToDraw, iconDest);
  needsRepaint = false;
}

void DrumPadComponent::resized() {
  needsRepaint = true;
}

void DrumPadComponent::mouseDown(const juce::MouseEvent& e) {
  if (e.mods.isLeftButtonDown()) {
    state->kbdState.noteOn(1, midiNote, 0.8f);
  }
}

void DrumPadComponent::mouseUp(const juce::MouseEvent& e) {
  if (e.mouseWasClicked()) {
    state->kbdState.noteOff(1, midiNote, 0.8f);
  }
}

//===================================================
