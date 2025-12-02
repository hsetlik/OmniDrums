//===================================================
#include "OmniDrums/GUI/Shared/Fonts.h"
#include "FontData.h"
#include "juce_graphics/juce_graphics.h"

namespace Fonts {

// helper for brevity
static juce::Font getFromBinary(const char* data, int size) {
  juce::Font font(juce::FontOptions(
      juce::Typeface::createSystemTypefaceFor(data, (size_t)size)));
  return font;
}

juce::Font getFont(const FontE& id) {
  static juce::Font iKenyanBI =
      getFromBinary(FontData::KenyanCoffeeBoldItalic_otf,
                    FontData::KenyanCoffeeBoldItalic_otfSize);
  static juce::Font iKenyanReg = getFromBinary(
      FontData::KenyanCoffeeRegular_otf, FontData::KenyanCoffeeRegular_otfSize);
  static juce::Font iRobotoLI = getFromBinary(
      FontData::RobotoLightItalic_ttf, FontData::RobotoLightItalic_ttfSize);
  static juce::Font iRobotoL =
      getFromBinary(FontData::RobotoLight_ttf, FontData::RobotoLight_ttfSize);
  static juce::Font iRobotoMI = getFromBinary(
      FontData::RobotoMediumItalic_ttf, FontData::RobotoMediumItalic_ttfSize);
  switch (id) {
    case KenyanBoldItalic:
      return iKenyanBI;
      break;
    case KenyanRegular:
      return iKenyanReg;
      break;
    case RobotoLightItalic:
      return iRobotoLI;
      break;
    case RobotoMedItalic:
      return iRobotoMI;
      break;
    case RobotoLight:
      return iRobotoL;
      break;
    default:
      jassert(false);
      return iRobotoLI;
      break;
  }
}

}  // namespace Fonts
