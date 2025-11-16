#pragma once
#include "../../Common.h"

typedef juce::Colour Color;

#define DECLARE_COLOR(name, r, g, b) const Color name = Color::fromRGB(r, g, b);

namespace UIColor {
DECLARE_COLOR(orangeLight, 226, 79, 54)
DECLARE_COLOR(bkgndGray, 51, 51, 51)
DECLARE_COLOR(borderGray, 102, 102, 102)

}  // namespace UIColor

#undef DECLARE_COLOR
