#pragma once
#include "../../Common.h"

// graphics-related type aliases can go here

typedef juce::Colour Color;
typedef juce::Point<float> fpoint_t;
typedef juce::Rectangle<float> frect_t;
typedef juce::Point<int> ipoint_t;
typedef juce::Rectangle<int> irect_t;

#define DECLARE_COLOR(name, r, g, b) const Color name = Color::fromRGB(r, g, b);

namespace UIColor {
DECLARE_COLOR(orangeLight, 226, 79, 54)
DECLARE_COLOR(bkgndGray, 51, 51, 51)
DECLARE_COLOR(shadowGray, 25, 25, 25)
DECLARE_COLOR(borderGray, 102, 102, 102)
DECLARE_COLOR(offWhite, 204, 204, 204)

}  // namespace UIColor

#undef DECLARE_COLOR
