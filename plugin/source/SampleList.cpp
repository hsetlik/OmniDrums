#include "OmniDrums/GUI/Browser/SampleList.h"
#include "OmniDrums/GUI/Browser/SearchHeader.h"
#include "OmniDrums/GUI/Shared/Color.h"
#include "OmniDrums/GUI/Shared/Fonts.h"
#include "OmniDrums/GUI/Shared/Images.h"
#include "OmniDrums/Identifiers.h"
#include "juce_core/juce_core.h"

LibEntryComponent::LibEntryComponent(OmniState* s, ValueTree tree)
    : state(s), sampleTree(tree) {
  jassert(sampleTree.hasType(ID::SAMPLE_LIB_ENTRY));
  fileName = sampleTree[ID::sampleFileName];
  String timeStr = sampleTree[ID::libSampleDateAdded];
  timeAdded = juce::Time::fromISO8601(timeStr);
  lengthMs = sampleTree[ID::libSampleLengthMs];
}

bool LibEntryComponent::compare(int sortMode,
                                const LibEntryComponent& other) const {
  SortModeE id = (SortModeE)sortMode;
  switch (id) {
    case sName:
      return fileName.compare(other.getFileName());
    case sDateAdded:
      return timeAdded.toMilliseconds() > other.getDateAdded().toMilliseconds();
    case sLength:
      return lengthMs > other.lengthMs;
    default:
      jassert(false);
  }
  return false;
}

static AttString getAttStringForEntry(const String& text, float yScale) {
  AttString str(text);
  str.setFont(
      Fonts::getFont(Fonts::RobotoLightItalic).withPointHeight(14.0f * yScale));
  str.setColour(UIColor::offWhite);
  str.setJustification(juce::Justification::centredLeft);
  return str;
}

static String getDateString(const juce::Time& time) {
  String str = String(time.getDayOfMonth()) + "-";
  str += String(time.getMonth() + 1) + "-";
  auto yearStr = String(time.getYear()).substring(2, 3);
  return str + yearStr;
}

void LibEntryComponent::paint(juce::Graphics& g) {
  juce::ignoreUnused(g);
  auto fBounds = getLocalBounds().toFloat();
  // TODO: draw the outline if this sample is selected
  const float yScale = fBounds.getHeight() / 25.0f;
  const float xScale = fBounds.getWidth() / 330.0f;
  const float strokeSize = 0.5f * yScale;
  // 1. draw the background and outine
  g.setColour(UIColor::bkgndGray);
  g.fillRect(fBounds);
  g.setColour(UIColor::shadowGray);
  g.drawRect(fBounds, strokeSize);
  // 2. draw the section dividers
  const float divWidth = 2.0f * xScale;
  const float divHeight = 23.0f * yScale;
  frect_t div1Bounds = {164.0f * xScale, yScale, divWidth, divHeight};
  g.setColour(UIColor::borderGray);
  g.fillRoundedRectangle(div1Bounds, divWidth / 2.0f);
  frect_t div2Bounds = {245.0f * xScale, yScale, divWidth, divHeight};
  g.fillRoundedRectangle(div2Bounds, divWidth / 2.0f);
  // 3. draw the text
  const float txtHeight = 23.0f * yScale;
  const float textY = 1.0f * yScale;
  frect_t nameBounds = {5.0f * xScale, textY, 156.0f * xScale, txtHeight};
  auto nameStr = getAttStringForEntry(fileName, yScale);
  nameStr.draw(g, nameBounds);
}
//===================================================
DropdownButton::DropdownButton() : juce::Button("dropdownToggle") {
  setClickingTogglesState(true);
}

void DropdownButton::paintButton(juce::Graphics& g,
                                 bool down,
                                 bool highlighted) {
  juce::ignoreUnused(down, highlighted);
  auto fBounds = getLocalBounds().toFloat();
  auto& img = getToggleState() ? Icons::getImage(Icons::DropdownOpen)
                               : Icons::getImage(Icons::DropdownClosed);
  g.drawImage(img, fBounds);
}

//===================================================

CategoryHeader::CategoryHeader(OmniState* s, int idx)
    : state(s), categoryID(idx) {
  addAndMakeVisible(btn);
  btn.onClick = [this]() {
    listIsOpen = !listIsOpen;
    auto* parentComp = getParentComponent();
    jassert(parentComp != nullptr);
    parentComp->resized();
  };
}

void CategoryHeader::paint(juce::Graphics& g) {}

//===================================================
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
