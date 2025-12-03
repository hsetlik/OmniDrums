#include "OmniDrums/GUI/Browser/SearchHeader.h"
#include "OmniDrums/GUI/Shared/Color.h"
#include "OmniDrums/GUI/Shared/Fonts.h"
#include "OmniDrums/GUI/Shared/Images.h"
#include "juce_core/juce_core.h"
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
  const float sideLength = std::min(fBounds.getHeight(), fBounds.getWidth());
  fBounds = fBounds.withSizeKeepingCentre(sideLength, sideLength);
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

  frect_t btnBounds = {308.5f * xRatio, 3.5f * yRatio, 15.0f * yRatio,
                       15.0f * yRatio};
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
