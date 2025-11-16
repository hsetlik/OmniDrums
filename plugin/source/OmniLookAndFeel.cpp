#include "OmniDrums/GUI/Shared/OmniLookAndFeel.h"

//===================================================
using schemeID = juce::LookAndFeel_V4::ColourScheme::UIColour;

static ColorScheme getColorScheme() {
  ColorScheme scheme;
  scheme.setUIColour(schemeID::windowBackground, UIColor::bkgndGray);
  scheme.setUIColour(schemeID::widgetBackground, UIColor::bkgndGray);
  scheme.setUIColour(schemeID::menuBackground, UIColor::bkgndGray);
  scheme.setUIColour(schemeID::outline, UIColor::borderGray);
  scheme.setUIColour(schemeID::defaultText, UIColor::offWhite);
  scheme.setUIColour(schemeID::menuText, UIColor::offWhite);
  scheme.setUIColour(schemeID::defaultFill, UIColor::borderGray);
  scheme.setUIColour(schemeID::highlightedText, UIColor::orangeLight);
  scheme.setUIColour(schemeID::highlightedFill, UIColor::orangeLight);
  return scheme;
}

//------------------------------------------------------

OmniLookAndFeel::OmniLookAndFeel() : juce::LookAndFeel_V4(getColorScheme()) {}
