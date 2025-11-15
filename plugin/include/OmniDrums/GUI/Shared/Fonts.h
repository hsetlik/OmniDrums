#pragma once
#include "juce_graphics/juce_graphics.h"

namespace Fonts {
enum FontE { KenyanBoldItalic, KenyanRegular, RobotoLightItalic };

juce::Font getFont(const FontE& id);
}  // namespace Fonts
