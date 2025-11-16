#pragma once
#include "Color.h"
#include "Fonts.h"
#include "Images.h"
#include "juce_gui_basics/juce_gui_basics.h"

typedef juce::LookAndFeel_V4::ColourScheme ColorScheme;

class OmniLookAndFeel : public juce::LookAndFeel_V4 {
public:
  OmniLookAndFeel();
  // Slider Methods====================================
  int getSliderThumbRadius(juce::Slider&) override;

  void drawLinearSlider(juce::Graphics&,
                        int x,
                        int y,
                        int width,
                        int height,
                        float sliderPos,
                        float minSliderPos,
                        float maxSliderPos,
                        juce::Slider::SliderStyle,
                        juce::Slider&) override;

  void drawRotarySlider(juce::Graphics&,
                        int x,
                        int y,
                        int width,
                        int height,
                        float sliderPosProportional,
                        float rotaryStartAngle,
                        float rotaryEndAngle,
                        juce::Slider&) override;

  // juce::Label* createSliderTextBox(juce::Slider&) override;
private:
  void drawLinearThumb(juce::Graphics& g, const frect_t& bounds, bool isActive);
};
