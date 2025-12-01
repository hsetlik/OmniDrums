#include "OmniDrums/GUI/SampleView/ChannelComponent.h"
#include "OmniDrums/GUI/SampleView/SampleView.h"
#include "OmniDrums/GUI/Shared/Color.h"
#include "OmniDrums/GUI/Shared/Fonts.h"
#include "OmniDrums/GUI/Shared/Images.h"
#include "OmniDrums/GUI/Shared/Util.h"
#include "OmniDrums/Identifiers.h"

DrumPadComponent::DrumPadComponent(OmniState* s, int idx)
    : state(s), channelIdx(idx), iconToDraw(juce::Image()) {
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
  if (!state->channelHasSample(channelIdx)) {
    return;
  }
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
  if (state->channelHasSample(channelIdx)) {
    auto bkgndImg = Assets::getImage(shouldDrawActive ? Assets::DrumPadOn
                                                      : Assets::DrumPadOff);
    g.drawImage(bkgndImg, fBounds);
    auto iconBounds = iconToDraw.getBounds().toFloat();
    const float aspectRatio = iconBounds.getWidth() / iconBounds.getHeight();
    const float iconWidth = fBounds.getWidth() * 0.65f;
    frect_t iconDest = {0.0f, 0.0f, iconWidth, iconWidth / aspectRatio};
    iconDest = iconDest.withCentre(fBounds.getCentre());
    g.drawImage(iconToDraw, iconDest);
  } else {
    g.setColour(UIColor::bkgndGray);
    g.fillRect(fBounds);
  }
  needsRepaint = false;
}

void DrumPadComponent::resized() {
  needsRepaint = true;
}

void DrumPadComponent::mouseDown(const juce::MouseEvent& e) {
  if (e.mods.isLeftButtonDown()) {
    state->kbdState.noteOn(1, midiNote, 0.8f);
    state->selectChannel(channelIdx);
    auto* parent = findParentComponentOfClass<ViewedChannelsComponent>();
    jassert(parent != nullptr);
    parent->repaint();
  }
}

void DrumPadComponent::mouseUp(const juce::MouseEvent& e) {
  if (e.mouseWasClicked()) {
    state->kbdState.noteOff(1, midiNote, 0.8f);
  }
}

//===================================================

SampleNameComponent::SampleNameComponent(OmniState* s, int idx)
    : state(s), channelIdx(idx) {
  state->samplesState.addListener(this);
}

SampleNameComponent::~SampleNameComponent() {
  state->samplesState.removeListener(this);
}

String SampleNameComponent::getLabelText() const {
  auto sampleTree = state->samplesState.getChildWithProperty(
      ID::sampleDrumChannel, channelIdx);
  if (!sampleTree.isValid()) {
    return "Empty";
  }
  int categIdx = sampleTree[ID::sampleDrumCategory];
  String name = sampleTree[ID::sampleFileName];
  return drumCategoryNames[categIdx] + " | " + name;
}

void SampleNameComponent::paint(juce::Graphics& g) {
  auto fBounds = getLocalBounds().toFloat();
  static const float cornerRadius = 4.0f;
  g.setColour(UIColor::shadowGray);
  g.fillRoundedRectangle(fBounds, cornerRadius);
  fBounds = fBounds.reduced(0.8f);
  g.setColour(UIColor::bkgndGray);
  g.fillRoundedRectangle(fBounds, cornerRadius);
  fBounds.removeFromLeft(fBounds.getHeight() * 0.5f);
  AttString aStr(getLabelText());
  aStr.setFont(Fonts::getFont(Fonts::RobotoLightItalic)
                   .withHeight(fBounds.getHeight() * 0.65f));
  aStr.setJustification(juce::Justification::centredLeft);
  auto textColor = (state->channelIsSelected(channelIdx)) ? UIColor::offWhite
                                                          : UIColor::borderGray;
  aStr.setColour(textColor);
  aStr.draw(g, fBounds);
}

//============================================================

OmniChannelComponent::OmniChannelComponent(OmniState* s, int chanIdx)
    : state(s),
      channelIdx(chanIdx),
      drumPad(s, chanIdx),
      nameComp(s, chanIdx),
      vuMeter(s, chanIdx) {
  // set up sliders
  gainSlider.setSliderStyle(juce::Slider::Rotary);
  gainSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
  const String gainID = ID::channelGain.toString() + String(channelIdx);
  gainAttach.reset(
      new apvts::SliderAttachment(state->audioState, gainID, gainSlider));
  addAndMakeVisible(gainSlider);

  panSlider.setSliderStyle(juce::Slider::Rotary);
  panSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
  const String panID = ID::channelPan.toString() + String(channelIdx);
  panAttach.reset(
      new apvts::SliderAttachment(state->audioState, panID, panSlider));
  addAndMakeVisible(panSlider);
  addAndMakeVisible(drumPad);
  addAndMakeVisible(nameComp);
  addAndMakeVisible(vuMeter);
}

void OmniChannelComponent::resized() {
  auto fBounds = getLocalBounds().toFloat();
  const float yScale = fBounds.getHeight() / 210.0f;
  const float xScale = fBounds.getWidth() / 296.0f;
  // as laid out in ChannelComponent.ai
  frect_t nameBounds = {4.0f, 8.0f, 170.0f, 18.0f};
  frect_t padBounds = {4.0f, 30.0f, 170.0f, 170.0f};
  frect_t gainBounds = {191.0f, 68.0f, 35.0f, 35.0f};
  frect_t panBounds = {190.0f, 157.0f, 35.0f, 35.0f};
  frect_t meterBounds = {248.0f, 21.0f, 35.0f, 185.0f};

  nameComp.setBounds(
      GraphicsUtil::scaledFor(nameBounds, xScale, yScale).toNearestInt());
  drumPad.setBounds(
      GraphicsUtil::scaledFor(padBounds, xScale, yScale).toNearestInt());
  gainSlider.setBounds(
      GraphicsUtil::scaledFor(gainBounds, xScale, yScale).toNearestInt());
  panSlider.setBounds(
      GraphicsUtil::scaledFor(panBounds, xScale, yScale).toNearestInt());
  vuMeter.setBounds(
      GraphicsUtil::scaledFor(meterBounds, xScale, yScale).toNearestInt());
}

static AttString getParamAttString(const String& text, float yScale) {
  AttString aStr(text);
  aStr.setWordWrap(AttString::none);
  aStr.setFont(
      Fonts::getFont(Fonts::RobotoLightItalic).withPointHeight(14.0f * yScale));
  aStr.setColour(UIColor::offWhite);
  aStr.setJustification(juce::Justification::centred);
  return aStr;
}

void OmniChannelComponent::paint(juce::Graphics& g) {
  auto fBounds = getLocalBounds().toFloat();
  const float yScale = fBounds.getHeight() / 210.0f;
  const float xScale = fBounds.getWidth() / 296.0f;
  if (state->channelIsSelected(channelIdx)) {
    g.setColour(UIColor::offWhite);
    g.fillRect(fBounds);
    fBounds = fBounds.reduced(0.8f);
  }
  g.setColour(UIColor::shadowGray);
  g.fillRect(fBounds);

  frect_t gainBounds = {194.0f * xScale, 44.0f * yScale, 28.0f * xScale,
                        16.0f * yScale};
  frect_t panBounds = {195.0f * xScale, 134.0f * yScale, 23.0f * xScale,
                       16.0f * yScale};

  static AttString gainAttStr = getParamAttString("Gain", yScale);
  static AttString panAttStr = getParamAttString("Pan", yScale);
  gainAttStr.draw(g, gainBounds);
  panAttStr.draw(g, panBounds);
}
