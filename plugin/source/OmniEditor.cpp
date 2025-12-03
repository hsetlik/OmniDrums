//===================================================
#include "OmniDrums/GUI/OmniEditor.h"
#include "OmniDrums/GUI/Shared/Color.h"
#include "OmniDrums/GUI/Shared/Fonts.h"
#include "OmniDrums/GUI/Shared/Images.h"
#include "OmniDrums/GUI/Shared/Util.h"
#include "OmniDrums/GUI/ViewSelector.h"
#include "juce_core/juce_core.h"

OmniEditor::OmniEditor(OmniState* s)
    : state(s), sampleView(s), viewSelector(s), browser(s) {
  setLookAndFeel(&lnf);
  state->graphingData.editorOpened();
  addAndMakeVisible(sampleView);
  addAndMakeVisible(viewSelector);
  viewSelector.addListener(this);
  addAndMakeVisible(browser);
  browser.setVisible(false);
  browser.setEnabled(false);
}

OmniEditor::~OmniEditor() {
  viewSelector.removeListener(this);
  setLookAndFeel(nullptr);
  state->graphingData.editorClosed();
  Icons::cleanupImages();
}

void OmniEditor::paint(juce::Graphics& g) {
  // fill the background
  auto fBounds = getLocalBounds().toFloat();
  const float yScale = fBounds.getHeight() / 1080.0f;
  const float xScale = fBounds.getWidth() / 1800.0f;
  g.setColour(UIColor::shadowGray);
  g.fillRect(fBounds);
  auto upperBounds = fBounds.removeFromTop(97.0f * yScale);
  g.setColour(UIColor::bkgndGray);
  g.fillRect(upperBounds);
  // draw the strings
  AttString nameStr("OmniDrums");
  nameStr.setFont(
      Fonts::getFont(Fonts::KenyanBoldItalic).withHeight(60.0f * yScale));
  nameStr.setJustification(juce::Justification::centredLeft);
  nameStr.setColour(UIColor::orangeLight);
  frect_t nameBounds = {11.0f * xScale, 11.0f * yScale, 231.0f * xScale,
                        66.0f * yScale};

  nameStr.draw(g, nameBounds);

  AttString versionStr("Version string goes here");
  versionStr.setFont(
      Fonts::getFont(Fonts::RobotoLightItalic).withHeight(14.0f * yScale));
  versionStr.setJustification(juce::Justification::centredLeft);
  versionStr.setColour(UIColor::borderGray);
  frect_t versionBounds = {15.0f * xScale, 72.0f * yScale, 148.0f * xScale,
                           16.0f * yScale};
  versionStr.draw(g, versionBounds);

  // draw the gradient border thing
  auto gradBounds = fBounds.removeFromTop(6.0f);
  g.drawImage(Assets::getImage(Assets::GradBar), gradBounds);
}

void OmniEditor::resized() {
  auto fBounds = getLocalBounds().toFloat();
  const float yScale = fBounds.getHeight() / 1080.0f;
  const float xScale = fBounds.getWidth() / 1800.0f;
  fBounds.removeFromTop(103.0f * yScale);
  auto vsBounds = fBounds.removeFromLeft(24.0f * xScale);
  viewSelector.setBounds(vsBounds.toNearestInt());
  if (state->isLibOpen()) {
    auto libBounds = fBounds.removeFromLeft(330 * xScale);
    browser.setBounds(libBounds.toNearestInt());
  }
  frect_t mainViewBounds = fBounds.removeFromTop(630.0f * yScale);
  sampleView.setBounds(mainViewBounds.toNearestInt());
}

void OmniEditor::viewSelected(const ViewE& id) {
  switch (id) {
    case vSamples:
      sampleView.setEnabled(true);
      sampleView.setVisible(true);
      resized();
      break;
    case vCompressor:
      sampleView.setEnabled(false);
      sampleView.setVisible(false);
      resized();
      break;
    case vRoom:
      sampleView.setEnabled(false);
      sampleView.setVisible(false);
      resized();
      break;
  }
}

void OmniEditor::libStateChanged(bool libOpen) {
  browser.setVisible(libOpen);
  browser.setEnabled(libOpen);
  resized();
}
