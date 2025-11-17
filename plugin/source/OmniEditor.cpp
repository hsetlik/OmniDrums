//===================================================
#include "OmniDrums/GUI/OmniEditor.h"
#include "OmniDrums/GUI/Shared/Color.h"
#include "OmniDrums/GUI/Shared/Fonts.h"
#include "OmniDrums/GUI/Shared/Images.h"

OmniEditor::OmniEditor(OmniState* s) : state(s) {
  setLookAndFeel(&lnf);
}

OmniEditor::~OmniEditor() {
  setLookAndFeel(nullptr);
}

void OmniEditor::paint(juce::Graphics& g) {
  // fill the background
  auto fBounds = getLocalBounds().toFloat();
  g.setColour(UIColor::shadowGray);
  g.fillRect(fBounds);
  auto upperBounds = fBounds.removeFromTop(97.0f);
  g.setColour(UIColor::bkgndGray);
  g.fillRect(upperBounds);
  // draw the strings
  AttString nameStr("OmniDrums");
  nameStr.setFont(Fonts::getFont(Fonts::KenyanBoldItalic).withHeight(60.0f));
  nameStr.setJustification(juce::Justification::centredLeft);
  nameStr.setColour(UIColor::orangeLight);
  frect_t nameBounds = {11.0f, 11.0f, 231.0f, 66.0f};
  nameStr.draw(g, nameBounds);

  AttString versionStr("Version string goes here");
  versionStr.setFont(
      Fonts::getFont(Fonts::RobotoLightItalic).withHeight(14.0f));
  versionStr.setJustification(juce::Justification::centredLeft);
  versionStr.setColour(UIColor::borderGray);
  frect_t versionBounds = {15.0f, 72.0f, 148.0f, 16.0f};
  versionStr.draw(g, versionBounds);

  // draw the gradient border thing
  auto gradBounds = fBounds.removeFromTop(6.0f);
  g.drawImage(Assets::getImage(Assets::GradBar), gradBounds);
}
