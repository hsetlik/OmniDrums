#include "OmniDrums/GUI/Shared/OmniLookAndFeel.h"
#include "OmniDrums/GUI/Shared/Color.h"
#include "OmniDrums/GUI/Shared/Images.h"
#include "juce_graphics/juce_graphics.h"
constexpr float ThumbAspectRatio = 1.0f / 3.0f;
constexpr float KnobTopAspectRatio = 24.0f / 35.0f;
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

OmniLookAndFeel::OmniLookAndFeel() : juce::LookAndFeel_V4(getColorScheme()) {
  // set some other color IDs
  setColour(juce::Slider::trackColourId, UIColor::shadowGray);
}

// Sliders-----------------------------------------------

int OmniLookAndFeel::getSliderThumbRadius(juce::Slider& slider) {
  return std::min(12, slider.isHorizontal()
                          ? (int)((float)slider.getHeight() * 0.5f)
                          : (int)((float)slider.getWidth() * 0.5f));
}

void OmniLookAndFeel::drawLinearSlider(juce::Graphics& g,
                                       int x,
                                       int y,
                                       int width,
                                       int height,
                                       float sliderPos,
                                       float minSliderPos,
                                       float maxSliderPos,
                                       juce::Slider::SliderStyle style,
                                       juce::Slider& slider) {
  if (slider.isBar()) {
    g.setColour(slider.findColour(juce::Slider::trackColourId));
    g.fillRect(slider.isHorizontal()
                   ? frect_t(static_cast<float>(x), (float)y + 0.5f,
                             sliderPos - (float)x, (float)height - 1.0f)
                   : frect_t((float)x + 0.5f, sliderPos, (float)width - 1.0f,
                             (float)y + ((float)height - sliderPos)));

    drawLinearSliderOutline(g, x, y, width, height, style, slider);
  } else {
    auto isTwoVal = (style == juce::Slider::SliderStyle::TwoValueVertical ||
                     style == juce::Slider::SliderStyle::TwoValueHorizontal);
    auto isThreeVal =
        (style == juce::Slider::SliderStyle::ThreeValueVertical ||
         style == juce::Slider::SliderStyle::ThreeValueHorizontal);

    auto trackWidth =
        std::min(6.0f, slider.isHorizontal() ? (float)height * 0.25f
                                             : (float)width * 0.25f);

    fpoint_t startPoint(
        slider.isHorizontal() ? (float)x : (float)x + (float)width * 0.5f,
        slider.isHorizontal() ? (float)y + (float)height * 0.5f
                              : (float)(height + y));

    fpoint_t endPoint(slider.isHorizontal() ? (float)(width + x) : startPoint.x,
                      slider.isHorizontal() ? startPoint.y : (float)y);

    juce::Path backgroundTrack;
    backgroundTrack.startNewSubPath(startPoint);
    backgroundTrack.lineTo(endPoint);
    g.setColour(slider.findColour(juce::Slider::trackColourId));
    g.strokePath(backgroundTrack, {trackWidth, juce::PathStrokeType::curved,
                                   juce::PathStrokeType::rounded});

    juce::Path valueTrack;
    fpoint_t minPoint, maxPoint, thumbPoint;

    if (isTwoVal || isThreeVal) {
      minPoint = {slider.isHorizontal() ? minSliderPos : (float)width * 0.5f,
                  slider.isHorizontal() ? (float)height * 0.5f : minSliderPos};

      if (isThreeVal)
        thumbPoint = {slider.isHorizontal() ? sliderPos : (float)width * 0.5f,
                      slider.isHorizontal() ? (float)height * 0.5f : sliderPos};

      maxPoint = {slider.isHorizontal() ? maxSliderPos : (float)width * 0.5f,
                  slider.isHorizontal() ? (float)height * 0.5f : maxSliderPos};
    } else {
      auto kx =
          slider.isHorizontal() ? sliderPos : ((float)x + (float)width * 0.5f);
      auto ky =
          slider.isHorizontal() ? ((float)y + (float)height * 0.5f) : sliderPos;

      minPoint = startPoint;
      maxPoint = {kx, ky};
    }
    // note that this returns the LARGER of the two sides
    auto thumbWidth = getSliderThumbRadius(slider);

    valueTrack.startNewSubPath(minPoint);
    valueTrack.lineTo(isThreeVal ? thumbPoint : maxPoint);
    g.setColour(slider.findColour(juce::Slider::trackColourId));
    g.strokePath(valueTrack, {trackWidth, juce::PathStrokeType::curved,
                              juce::PathStrokeType::rounded});

    if (!isTwoVal) {
      frect_t thumbBounds =
          slider.isHorizontal()
              ? frect_t((float)thumbWidth, (float)thumbWidth * ThumbAspectRatio)
              : frect_t((float)thumbWidth * ThumbAspectRatio,
                        (float)thumbWidth);
      g.setColour(slider.findColour(juce::Slider::thumbColourId));
      g.fillEllipse(frect_t(static_cast<float>(thumbWidth),
                            static_cast<float>(thumbWidth))
                        .withCentre(isThreeVal ? thumbPoint : maxPoint));
      thumbBounds = thumbBounds.withCentre(isThreeVal ? thumbPoint : maxPoint);
      drawLinearThumb(g, thumbBounds, slider.isEnabled());
    }

    if (isTwoVal || isThreeVal) {
      auto sr = juce::jmin(
          trackWidth,
          (slider.isHorizontal() ? (float)height : (float)width) * 0.4f);
      auto pointerColour = slider.findColour(juce::Slider::thumbColourId);

      if (slider.isHorizontal()) {
        drawPointer(g, minSliderPos - sr,
                    juce::jmax(0.0f, (float)y + (float)height * 0.5f -
                                         trackWidth * 2.0f),
                    trackWidth * 2.0f, pointerColour, 2);

        drawPointer(g, maxSliderPos - trackWidth,
                    juce::jmin((float)(y + height) - trackWidth * 2.0f,
                               (float)y + (float)height * 0.5f),
                    trackWidth * 2.0f, pointerColour, 4);
      } else {
        drawPointer(g,
                    juce::jmax(0.0f, (float)x + (float)width * 0.5f -
                                         trackWidth * 2.0f),
                    minSliderPos - trackWidth, trackWidth * 2.0f, pointerColour,
                    1);

        drawPointer(g,
                    juce::jmin((float)(x + width) - trackWidth * 2.0f,
                               (float)x + (float)width * 0.5f),
                    maxSliderPos - sr, trackWidth * 2.0f, pointerColour, 3);
      }
    }
  }
}

void OmniLookAndFeel::drawLinearThumb(juce::Graphics& g,
                                      const frect_t& bounds,
                                      bool isActive) {
  auto img = Assets::getImage(isActive ? Assets::ThumbOn : Assets::ThumbOff);
  g.drawImage(img, bounds);
}

void OmniLookAndFeel::drawRotarySlider(juce::Graphics& g,
                                       int x,
                                       int y,
                                       int width,
                                       int height,
                                       float sliderPosProportional,
                                       float rotaryStartAngle,
                                       float rotaryEndAngle,
                                       juce::Slider& slider) {
  juce::ignoreUnused(slider);
  if (width != height) {
    if (width < height)
      height = width;
    else
      width = height;
  }
  irect_t iBounds(x, y, width, height);
  auto sideLength = std::min(width, height);
  iBounds = iBounds.withSizeKeepingCentre(sideLength, sideLength);
  auto bounds = iBounds.toFloat();
  // 1. draw the background image
  auto bkgndImg = Assets::getImage(Assets::KnobBkgnd);
  g.drawImage(bkgndImg, bounds);
  // 2. find the bounds for the knob top
  auto topBounds =
      bounds.withSizeKeepingCentre(bounds.getWidth() * KnobTopAspectRatio,
                                   bounds.getHeight() * KnobTopAspectRatio);
  // g.setColour(juce::Colours::chartreuse);
  // g.fillRect(topBounds);
  // angle and draw the knob top
  // const float scale = topBounds.getWidth() / 25.0f;
  auto topImg = Assets::getImage(Assets::KnobTop);
  auto endAngle =
      flerp(rotaryStartAngle, rotaryEndAngle, sliderPosProportional);
  auto transform = juce::AffineTransform::rotation(
      endAngle, topBounds.getCentreX(), topBounds.getCentreY());
  // .followedBy(juce::AffineTransform::scale(
  //     scale, scale, topBounds.getCentreX(), topBounds.getCentreY()));
  // auto topImg = Assets::getImage(Assets::KnobTop);
  g.addTransform(transform);
  g.drawImage(topImg, topBounds);
}
