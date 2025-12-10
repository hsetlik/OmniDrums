#include "OmniDrums/GUI/Browser/SampleList.h"
#include "OmniDrums/Audio/SamplePlayer.h"
#include "OmniDrums/Common.h"
#include "OmniDrums/GUI/Browser/SearchHeader.h"
#include "OmniDrums/GUI/Shared/Color.h"
#include "OmniDrums/GUI/Shared/Fonts.h"
#include "OmniDrums/GUI/Shared/Images.h"
#include "OmniDrums/Identifiers.h"
#include "juce_core/juce_core.h"
#include "juce_gui_basics/juce_gui_basics.h"

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

juce::File LibEntryComponent::getSampleFile() const {
  return state->sampleLib->fileForSample(sampleTree);
}

void LibEntryComponent::mouseDown(const juce::MouseEvent& e) {
  juce::ignoreUnused(e);
  auto* container =
      juce::DragAndDropContainer::findParentDragContainerFor(this);
  jassert(container != nullptr);
  // get this sample's path relative to the lib folder
  auto libFolder = OmniSampleLibrary::getSampleLibFolder();
  auto sampleFile = state->sampleLib->fileForSample(sampleTree);
  const String path = sampleFile.getRelativePathFrom(libFolder);
  container->startDragging(path, this);
}

void LibEntryComponent::mouseUp(const juce::MouseEvent& e) {
  if (e.mouseWasClicked()) {
    auto* parent = findParentComponentOfClass<SampleBrowser>();
    jassert(parent != nullptr);
    parent->setSelected(this);
  }
}

static AttString getAttStringForEntry(const String& text, float yScale) {
  AttString str(text);
  str.setFont(
      Fonts::getFont(Fonts::RobotoLightItalic).withHeight(10.0f * yScale));
  str.setColour(UIColor::offWhite);
  str.setJustification(juce::Justification::centredLeft);
  return str;
}

static String getDateString(const juce::Time& time) {
  String str = String(time.getDayOfMonth()) + "-";
  str += String(time.getMonth() + 1) + "-";
  auto yearStr = String(time.getYear()).substring(2, 4);
  return str + yearStr;
}

static String getLengthString(int lengthMs) {
  const int fraction = lengthMs % 1000;
  const int secs = (lengthMs - fraction) / 1000;
  auto fracString = String((float)fraction / 100.0f);
  if (fracString.length() > 2) {
    fracString = fracString.substring(0, 2);
  }
  return String(secs) + "." + fracString + "s";
}

void LibEntryComponent::paint(juce::Graphics& g) {
  auto fBounds = getLocalBounds().toFloat();
  const float yScale = fBounds.getHeight() / 25.0f;
  const float xScale = fBounds.getWidth() / 330.0f;
  const float strokeSize = 0.5f * yScale;
  const bool selected = isSelected();
  // 1. draw the background and outine
  auto outlineColor = selected ? UIColor::offWhite : UIColor::bkgndGray;
  g.setColour(UIColor::bkgndGray);
  g.fillRect(fBounds);
  g.setColour(outlineColor);
  g.drawRect(fBounds, strokeSize);
  // 2. draw the section dividers
  const float divWidth = 2.0f * xScale;
  const float divHeight = 23.0f * yScale;
  frect_t div1Bounds = {164.0f * xScale, yScale, divWidth, divHeight};
  auto divColor = selected ? UIColor::offWhite : UIColor::borderGray;
  g.setColour(divColor);
  g.fillRoundedRectangle(div1Bounds, divWidth / 2.0f);
  frect_t div2Bounds = {265.0f * xScale, yScale, divWidth, divHeight};
  g.fillRoundedRectangle(div2Bounds, divWidth / 2.0f);
  // 3. draw the text
  const float txtHeight = 18.0f * yScale;
  const float textY = 3.0f * yScale;
  frect_t nameBounds = {5.0f * xScale, textY, 156.0f * xScale, txtHeight};
  auto nameStr = getAttStringForEntry(fileName, yScale);
  nameStr.draw(g, nameBounds);
  frect_t dateBounds = {172.0f * xScale, textY, 73.0f * xScale, txtHeight};
  auto dateStr = getAttStringForEntry(getDateString(timeAdded), yScale);
  dateStr.draw(g, dateBounds);
  frect_t lengthBounds = {268.0f * xScale, textY, 62.0f * xScale, txtHeight};
  auto lengthStr = getAttStringForEntry(getLengthString(lengthMs), yScale);
  lengthStr.draw(g, lengthBounds);
}

void LibEntryComponent::enablementChanged() {
  if (!isEnabled()) {
    auto* parent = findParentComponentOfClass<SampleBrowser>();
    if (parent != nullptr && isSelected())
      parent->setSelected(nullptr);
  }
}

bool LibEntryComponent::isSelected() const {
  auto* parent = findParentComponentOfClass<SampleBrowser>();
  jassert(parent != nullptr);
  return parent->isSelected(this);
}
//===================================================

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
    auto* parentComp = findParentComponentOfClass<SampleBrowser>();
    jassert(parentComp != nullptr);
    parentComp->resized();
  };
}

void CategoryHeader::paint(juce::Graphics& g) {
  auto fBounds = getLocalBounds().toFloat();
  const float yScale = fBounds.getHeight() / 25.0f;
  const float xScale = fBounds.getWidth() / 330.0f;
  const float strokeSize = 0.5f * yScale;
  // 1. draw the background and outine
  g.setColour(UIColor::shadowGray);
  g.fillRect(fBounds);
  g.setColour(UIColor::borderGray);
  g.drawRect(fBounds, strokeSize);
  // 2. draw the text
  AttString aStr(drumCategoryNames[categoryID]);
  auto font =
      Fonts::getFont(Fonts::RobotoMedItalic).withPointHeight(14.0f * yScale);
  aStr.setFont(font);
  aStr.setColour(UIColor::borderGray);
  aStr.setJustification(juce::Justification::centredLeft);
  const float txtHeight = 23.0f * yScale;
  const float textY = 1.0f * yScale;
  frect_t nameBounds = {5.0f * xScale, textY, 280.0f * xScale, txtHeight};
  aStr.draw(g, nameBounds);
}

void CategoryHeader::resized() {
  auto fBounds = getLocalBounds().toFloat();
  const float yScale = fBounds.getHeight() / 25.0f;
  const float xScale = fBounds.getWidth() / 330.0f;
  frect_t btnBounds = {312.0f * xScale, 6.0f * yScale, 13.0f * xScale,
                       13.0f * yScale};
  btn.setBounds(btnBounds.toNearestInt());
}

//===================================================
CategoryHolder::CategoryHolder(OmniState* s, SearchHeader* h, int idx)
    : state(s), searchHeader(h), categoryID(idx), header(s, idx) {
  addAndMakeVisible(header);
  searchHeader->addListener(this);
  // 1. create components for each entry
  auto categTree = state->sampleLib->getCategoryTree(categoryID);
  for (int i = 0; i < categTree.getNumChildren(); ++i) {
    auto sampleTree = categTree.getChild(i);
    auto* entry = entries.add(new LibEntryComponent(state, sampleTree));
    addAndMakeVisible(entry);
  }
  resized();
}
CategoryHolder::~CategoryHolder() {
  searchHeader->removeListener(this);
}

std::vector<LibEntryComponent*> CategoryHolder::getVisibleEntries() const {
  std::vector<LibEntryComponent*> visible = {};
  auto searchString = currentSearchText.toLowerCase();
  if (header.isOpen()) {
    for (auto* entry : entries) {
      auto lowerCaseName = entry->getFileName().toLowerCase();
      if (lowerCaseName.contains(searchString)) {
        visible.push_back(entry);
      }
    }
  }
  return visible;
}

void CategoryHolder::sortEntries(std::vector<LibEntryComponent*>& list) const {
  std::sort(list.begin(), list.end(),
            [this](LibEntryComponent* a, LibEntryComponent* b) {
              return a->compare(this->currentSortMode, *b);
            });
}

int CategoryHolder::numBarsVisible() const {
  auto vis = getVisibleEntries();
  return 1 + (int)vis.size();
}

void CategoryHolder::searchStateChanged(const LibSearchState& search) {
  currentSortMode = search.sortMode;
  currentSearchText = search.searchText;
  resized();
}

void CategoryHolder::resized() {
  // 1. set all entries invisible/disabled to start
  for (auto* e : entries) {
    e->setVisible(false);
    e->setEnabled(false);
  }
  // 2. determine which should be visible, reactivate then sort them
  auto visEntries = getVisibleEntries();
  sortEntries(visEntries);
  // 3. place stuff
  const float barHeight = yScale * 25.0f;
  const float barWidth = xScale * 330.0f;
  frect_t headerBounds = {0.0f, 0.0f, barWidth, barHeight};
  header.setBounds(headerBounds.toNearestInt());
  float y = barHeight;
  for (auto* e : visEntries) {
    frect_t bounds = {0.0f, y, barWidth, barHeight};
    e->setVisible(true);
    e->setEnabled(true);
    e->setBounds(bounds.toNearestInt());
    y += barHeight;
  }
}

//===================================================
ViewedSampleList::ViewedSampleList(OmniState* s, SearchHeader* h) {
  for (int i = 0; i < NUM_DRUM_CATEGORIES; ++i) {
    auto* category = categories.add(new CategoryHolder(s, h, i));
    addAndMakeVisible(category);
  }
}

void ViewedSampleList::setScales(float x, float y) {
  xScale = x;
  yScale = y;
  for (auto* c : categories) {
    c->setScales(xScale, yScale);
  }
}

void ViewedSampleList::resized() {
  const float barWidth = 330.0f * xScale;
  const float barHeight = 25.0f * yScale;
  float yPos = 0.0f;
  for (auto* c : categories) {
    float currentHeight = barHeight * (float)c->numBarsVisible();
    frect_t bounds = {0.0f, yPos, barWidth, currentHeight};
    c->setBounds(bounds.toNearestInt());
    yPos += currentHeight;
  }
  setSize((int)barWidth, (int)yPos);
}
//===================================================

DetailView::PlaybackButton::PlaybackButton() : juce::Button("playback") {}

void DetailView::PlaybackButton::paintButton(juce::Graphics& g,
                                             bool down,
                                             bool highlighted) {
  juce::ignoreUnused(highlighted);
  auto& img = down ? Icons::getImage(Icons::PlaybackOn)
                   : Icons::getImage(Icons::PlaybackOff);
  auto fBounds = getLocalBounds().toFloat();
  g.drawImage(img, fBounds);
}

DetailView::DetailView(OmniState* s)
    : state(s), waveImg(juce::Image::ARGB, waveWidth, 110, true) {
  manager.registerBasicFormats();
  addAndMakeVisible(playBtn);
  playBtn.setVisible(false);
  playBtn.setEnabled(false);
  playBtn.onClick = [this]() {
    if (entry != nullptr) {
      auto file = entry->getSampleFile();
      state->previewSample(file);
    }
  };
}

void DetailView::setSelectedSample(LibEntryComponent* comp) {
  entry = comp;
  playBtn.setVisible(entry != nullptr);
  playBtn.setEnabled(entry != nullptr);
  if (entry != nullptr) {
    drawWaveformImage();
  }
  repaint();
}

static juce::Path createMonoPath(juce::AudioBuffer<float>& buf, int numPixels) {
  juce::Path p;
  p.startNewSubPath(0.0f, 55.0f);
  for (int i = 0; i < numPixels; ++i) {
    auto fPos = (float)i / (float)numPixels;
    int sampleIdx = (int)(fPos * (float)buf.getNumSamples());
    float yPos = 55.0f + (buf.getSample(0, sampleIdx) * 50.0f);
    jassert(yPos >= 0.0f && yPos <= 145.0f);
    p.lineTo((float)i, yPos);
  }
  return p;
}

static juce::Path createStereoPath(juce::AudioBuffer<float>& buf,
                                   int numPixels) {
  juce::Path p;
  p.startNewSubPath(0.0f, 55.0f);
  for (int i = 0; i < numPixels; ++i) {
    auto fPos = (float)i / (float)numPixels;
    int sampleIdx = (int)(fPos * (float)buf.getNumSamples());
    const float normSample =
        flerp(buf.getSample(0, sampleIdx), buf.getSample(1, sampleIdx), 0.5f);
    const float yPos = 55.0f + (normSample * 50.0f);
    p.lineTo((float)i, yPos);
  }
  return p;
}

void DetailView::drawWaveformImage() {
  // 1. grip the file and load it into a buffer
  auto file = entry->getSampleFile();
  jassert(file.existsAsFile());
  auto reader = juce::rawToUniquePtr(manager.createReaderFor(file));
  AudioBufF buffer((int)reader->numChannels, (int)reader->lengthInSamples);
  jassert(reader->read(&buffer, 0, buffer.getNumSamples(), 0, true,
                       buffer.getNumChannels() > 1));
  // 2. draw the background
  juce::Graphics g(waveImg);
  irect_t bounds = {0, 0, waveWidth, 110};
  g.setColour(UIColor::shadowGray);
  g.fillRect(bounds);
  juce::Path centerLine;
  centerLine.startNewSubPath(0, 55);
  centerLine.lineTo(waveWidth, 55);
  g.setColour(UIColor::borderGray);
  g.strokePath(centerLine, juce::PathStrokeType(0.5f));

  auto p = buffer.getNumChannels() > 1 ? createStereoPath(buffer, waveWidth)
                                       : createMonoPath(buffer, waveWidth);
  g.setColour(UIColor::greenLight.withAlpha(0.9f));
  g.strokePath(p, juce::PathStrokeType(0.7f));
}

void DetailView::resized() {
  auto fBounds = getLocalBounds().toFloat();
  const float xScale = fBounds.getWidth() / 330.0f;
  const float yScale = fBounds.getHeight() / 145.0f;
  frect_t btnBounds = {3.0f * xScale, 28.0f * yScale, 30.0f * xScale,
                       110.0f * yScale};
  playBtn.setBounds(btnBounds.toNearestInt());
}

void DetailView::paint(juce::Graphics& g) {
  auto fBounds = getLocalBounds().toFloat();
  const float xScale = fBounds.getWidth() / 330.0f;
  const float yScale = fBounds.getHeight() / 145.0f;
  // 1. fill the backdround
  g.setColour(UIColor::bkgndGray);
  g.fillRect(fBounds);
  if (entry != nullptr) {
    // 2. draw the divider thing
    const float divHeight = 2.0f * yScale;
    frect_t divBounds = {3.0f * xScale, 20.0f * yScale, 324.0f * xScale,
                         divHeight};
    g.setColour(UIColor::borderGray);
    g.fillRoundedRectangle(divBounds, yScale);
    // 3. draw the file name
    frect_t textBounds = {3.0f * xScale, 3.0f * yScale, 324.0f * xScale,
                          14.0f * yScale};
    AttString aStr(entry->getFileName());
    auto font = Fonts::getFont(Fonts::RobotoLightItalic)
                    .withHeight(textBounds.getHeight());
    aStr.setFont(font);
    aStr.setColour(UIColor::offWhite);
    aStr.setJustification(juce::Justification::centredLeft);
    aStr.draw(g, textBounds);
    // 4. draw the waveform
    frect_t waveBounds = {36.0f * xScale, 28.0f * yScale, 285.0f * xScale,
                          110.0f * yScale};
    g.drawImage(waveImg, waveBounds);
  }
}

//===================================================
SampleBrowser::SampleBrowser(OmniState* s)
    : state(s), list(s, &header), detailView(s) {
  addAndMakeVisible(header);
  addAndMakeVisible(detailView);
  header.addListener(this);
  vpt.setViewedComponent(&list, false);
  vpt.setViewPosition(0, 0);
  vpt.setInterceptsMouseClicks(true, true);
  vpt.setScrollBarsShown(true, false, false, false);
  addAndMakeVisible(vpt);
  list.resized();
}

SampleBrowser::~SampleBrowser() {
  header.removeListener(this);
}

void SampleBrowser::searchStateChanged(const LibSearchState& searchState) {
  juce::ignoreUnused(searchState);
}

void SampleBrowser::setSelected(LibEntryComponent* e) {
  selectedEntry = e;
  detailView.setSelectedSample(e);
  repaint();
}

void SampleBrowser::resized() {
  auto fBounds = getLocalBounds().toFloat();
  const float yScale = fBounds.getHeight() / 610.0f;
  const float xScale = fBounds.getWidth() / 330.0f;
  auto headerBounds = fBounds.removeFromTop(48.0f * yScale);
  header.setBounds(headerBounds.toNearestInt());
  auto detailBounds = fBounds.removeFromBottom(145.0f * yScale);
  detailView.setBounds(detailBounds.toNearestInt());
  list.setScales(xScale, yScale);
  list.resized();
  vpt.setBounds(fBounds.toNearestInt());
}
