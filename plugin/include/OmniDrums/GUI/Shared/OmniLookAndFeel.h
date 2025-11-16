#pragma once
#include "Color.h"
#include "Fonts.h"
#include "Images.h"
#include "juce_gui_basics/juce_gui_basics.h"

typedef juce::LookAndFeel_V4::ColourScheme ColorScheme;

class OmniLookAndFeel : public juce::LookAndFeel_V4 {
public:
  OmniLookAndFeel();
};
