#include "OmniDrums/GUI/Browser/SearchHeader.h"
#include "OmniDrums/GUI/Shared/Color.h"
#include "OmniDrums/GUI/Shared/Fonts.h"
#include "OmniDrums/GUI/Shared/Images.h"
#include "juce_core/juce_core.h"
#include "juce_events/juce_events.h"
#include "juce_gui_basics/juce_gui_basics.h"
//===================================================

SearchBarLnF::SearchBarLnF() : juce::LookAndFeel_V4() {}

void SearchBarLnF::fillTextEditorBackground(juce::Graphics& g,
                                            int width,
                                            int height,
                                            juce::TextEditor& te) {
  frect_t fBounds = {0.0f, 0.0f, (float)width, (float)height};
  g.setColour(UIColor::shadowGray);
  g.fillRect(fBounds);
  const float yRatio = (float)height / 13.0f;
  if (te.getText().length() < 1) {
    frect_t searchBounds = {0.0f, 0.0f, yRatio * 13.0f, yRatio * 13.0f};
    auto& img = Icons::getImage(Icons::Search);
    g.drawImage(img, searchBounds);
  }
}

void SearchBarLnF::drawTextEditorOutline(juce::Graphics& g,
                                         int width,
                                         int height,
                                         juce::TextEditor& te) {
  juce::ignoreUnused(g, width, height, te);
}

void ClearButton::paintButton(juce::Graphics& g, bool highlighted, bool down) {
  juce::ignoreUnused(highlighted, down);
  auto fBounds = getLocalBounds().toFloat();
  auto& img = Icons::getImage(Icons::Close);
  g.drawImage(img, fBounds);
}

LibrarySearchBar::LibrarySearchBar() {
  editor.setLookAndFeel(&lnf);
  auto font = Fonts::getFont(Fonts::RobotoLight).withPointHeight(14.0f);
  editor.setFont(font);
  addAndMakeVisible(editor);
  addAndMakeVisible(clearBtn);
  clearBtn.onClick = [this]() { this->editor.setText("", true); };
}

LibrarySearchBar::~LibrarySearchBar() {
  editor.setLookAndFeel(nullptr);
}

void LibrarySearchBar::resized() {
  auto fBounds = getLocalBounds().toFloat();
  const float yRatio = fBounds.getHeight() / 22.0f;
  const float xRatio = fBounds.getWidth() / 327.0f;
  frect_t editorBounds = {2.5f * xRatio, 4.0f * yRatio, 303.0f * xRatio,
                          14.0f * yRatio};
  editor.setBounds(editorBounds.toNearestInt());

  frect_t btnBounds = {308.0f * xRatio, 3.5f * yRatio, 15.0f * xRatio,
                       15.0f * xRatio};
  btnBounds =
      btnBounds.withCentre({btnBounds.getCentreX(), fBounds.getCentreY()});
  clearBtn.setBounds(btnBounds.toNearestInt());
}

void LibrarySearchBar::paint(juce::Graphics& g) {
  auto fBounds = getLocalBounds().toFloat();
  const float yRatio = fBounds.getHeight() / 22.0f;
  g.setColour(UIColor::shadowGray);
  g.fillRoundedRectangle(fBounds, 3.5f * yRatio);
  g.setColour(UIColor::borderGray);
  g.drawRoundedRectangle(fBounds, 3.5f * yRatio, 0.5f * yRatio);
}

//==================================================================

void SearchHeader::addListener(Listener* l) {
  listeners.push_back(l);
}

void SearchHeader::removeListener(Listener* l) {
  for (auto it = listeners.begin(); it != listeners.end(); ++it) {
    if (*it == l) {
      listeners.erase(it);
      return;
    }
  }
}

//----------------------------------------------------------------

SearchHeader::ColumnHeadBtn::ColumnHeadBtn(const String& txt)
    : juce::Button(txt), columnText(txt) {
  setClickingTogglesState(true);
  setRadioGroupId(1);
}

void SearchHeader::ColumnHeadBtn::paintButton(juce::Graphics& g,
                                              bool highlighted,
                                              bool down) {
  juce::ignoreUnused(highlighted, down);
  auto fBounds = getLocalBounds().toFloat();
  const float yRatio = fBounds.getHeight() / 18.0f;
  const float fontSize = 14.0f * yRatio;
  auto font =
      getToggleState()
          ? Fonts::getFont(Fonts::RobotoMedItalic).withPointHeight(fontSize)
          : Fonts::getFont(Fonts::RobotoLightItalic).withPointHeight(fontSize);
  AttString aStr(columnText);
  aStr.setFont(font);
  aStr.setColour(UIColor::borderGray);
  aStr.setJustification(juce::Justification::centredLeft);
  aStr.draw(g, fBounds);
}

//----------------------------------------------------------------

SearchHeader::SearchHeader()
    : nameBtn("Name"), dateBtn("Added"), lengthBtn("Length") {
  // 1. add components
  nameBtn.setToggleState(true, juce::sendNotification);
  addAndMakeVisible(nameBtn);
  addAndMakeVisible(dateBtn);
  addAndMakeVisible(lengthBtn);
  addAndMakeVisible(searchBar);
  //  2. attach search bar listener
  searchBar.editor.addListener(this);
  // 3. set up the button lambdas
  nameBtn.onClick = [this]() {
    this->searchState.sortMode = (int)SortModeE::sName;
    this->notifyListeners();
  };
  dateBtn.onClick = [this]() {
    this->searchState.sortMode = (int)SortModeE::sDateAdded;
    this->notifyListeners();
  };
  lengthBtn.onClick = [this]() {
    this->searchState.sortMode = (int)SortModeE::sLength;
    this->notifyListeners();
  };
  // nameBtn.triggerClick();
}

SearchHeader::~SearchHeader() {
  searchBar.editor.removeListener(this);
}

void SearchHeader::textEditorTextChanged(juce::TextEditor& te) {
  searchState.searchText = te.getText();
  notifyListeners();
}

void SearchHeader::notifyListeners() {
  for (auto* l : listeners) {
    l->searchStateChanged(searchState);
  }
  repaint();
}

void SearchHeader::resized() {
  auto fBounds = getLocalBounds().toFloat();
  const float xScale = fBounds.getWidth() / 330.0f;
  const float yScale = fBounds.getHeight() / 48.0f;
  frect_t nBounds = {6.0f * xScale, 4.0f * yScale, 155.0f * xScale,
                     18.0f * yScale};
  frect_t dBounds = {170.0f * xScale, 4.0f * yScale, 72.0f * xScale,
                     18.0f * yScale};
  frect_t lBounds = {250.0f * xScale, 4.0f * yScale, 76.0f * xScale,
                     18.0f * yScale};
  frect_t searchBounds = {2.0f * xScale, 25.0f * yScale, 327.0f * xScale,
                          22.0f * yScale};
  nameBtn.setBounds(nBounds.toNearestInt());
  dateBtn.setBounds(dBounds.toNearestInt());
  lengthBtn.setBounds(lBounds.toNearestInt());
  searchBar.setBounds(searchBounds.toNearestInt());
}

void SearchHeader::paint(juce::Graphics& g) {
  auto fBounds = getLocalBounds().toFloat();
  const float xScale = fBounds.getWidth() / 330.0f;
  const float yScale = fBounds.getHeight() / 48.0f;
  g.setColour(UIColor::bkgndGray);
  g.fillRect(fBounds);

  frect_t dBounds1 = {164.0f * xScale, yScale, 2.0f * xScale, 23.0f * yScale};
  frect_t dBounds2 = {245.0f * xScale, yScale, 2.0f * xScale, 23.0f * yScale};
  g.setColour(UIColor::borderGray);
  g.fillRoundedRectangle(dBounds1, xScale);
  g.fillRoundedRectangle(dBounds2, xScale);
}
