#include "OmniDrums/GUI/Shared/OmniLookAndFeel.h"

//===================================================
using schemeID = juce::LookAndFeel_V4::ColourScheme::UIColour;

static ColorScheme getColorScheme() {
  ColorScheme scheme = juce::LookAndFeel_V4::getDarkColourScheme();
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

// Sliders-----------------------------------------------

int OmniLookAndFeel::getSliderThumbRadius(juce::Slider& slider) {
  return std::min(12, slider.isHorizontal()
                          ? (int)((float)slider.getHeight() * 0.5f)
                          : (int)((float)slider.getWidth() * 0.5f));
}

// void OmniLookAndFeel::drawLinearSlider(juce::Graphics& g,
//                                        int x,
//                                        int y,
//                                        int width,
//                                        int height,
//                                        float sliderPos,
//                                        float minSliderPos,
//                                        float maxSliderPos,
//                                        juce::Slider::SliderStyle style,
//                                        juce::Slider& slider) {}
