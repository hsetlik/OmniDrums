//===================================================
#include "OmniDrums/GUI/Shared/Images.h"
#include "AssetData.h"
#include "IconData.h"

namespace Assets {
juce::Image getImage(const AssetE& id) {
  static juce::Image iGradBar = juce::ImageFileFormat::loadFrom(
      AssetData::GradBar_6x1800_png, AssetData::GradBar_6x1800_pngSize);
  static juce::Image iKnobBkgnd = juce::ImageFileFormat::loadFrom(
      AssetData::KnobBkgnd_35x35_png, AssetData::KnobBkgnd_35x35_pngSize);
  static juce::Image iKnobTop = juce::ImageFileFormat::loadFrom(
      AssetData::KnobTop_24x24_png, AssetData::KnobTop_24x24_pngSize);
  switch (id) {
    case GradBar:
      return iGradBar;
      break;
    case KnobBkgnd:
      return iKnobBkgnd;
      break;
    case KnobTop:
      return iKnobTop;
      break;
    default:
      jassert(false);
      return iKnobTop;
      break;
  }
}
}  // namespace Assets

namespace Icons {
juce::Image getImage(const IconE& id) {
  static juce::Image iKick = juce::ImageFileFormat::loadFrom(
      IconData::Kick_256_png, IconData::Kick_256_pngSize);
  static juce::Image iSnare = juce::ImageFileFormat::loadFrom(
      IconData::Snare_256_png, IconData::Snare_256_pngSize);
  static juce::Image iOpenHat = juce::ImageFileFormat::loadFrom(
      IconData::OpenHat_256_png, IconData::OpenHat_256_pngSize);
  static juce::Image iClosedHat = juce::ImageFileFormat::loadFrom(
      IconData::ClosedHat_256_png, IconData::ClosedHat_256_pngSize);
  static juce::Image iRide = juce::ImageFileFormat::loadFrom(
      IconData::Ride_256_png, IconData::Ride_256_pngSize);
  static juce::Image iTom = juce::ImageFileFormat::loadFrom(
      IconData::Tom_256_png, IconData::Tom_256_pngSize);
  static juce::Image iLibOpen = juce::ImageFileFormat::loadFrom(
      IconData::LibOpen_256_png, IconData::LibOpen_256_pngSize);
  static juce::Image iLibClosed = juce::ImageFileFormat::loadFrom(
      IconData::LibClosed_256_png, IconData::LibClosed_256_pngSize);
  switch (id) {
    case Kick:
      return iKick;
      break;
    case Snare:
      return iSnare;
      break;
    case OpenHat:
      return iOpenHat;
      break;
    case ClosedHat:
      return iClosedHat;
      break;
    case Ride:
      return iRide;
      break;
    case Tom:
      return iTom;
    case LibOpen:
      return iLibOpen;
    case LibClosed:
      return iLibClosed;
    default:
      jassert(false);
      return iKick;
      break;
  }
}
}  // namespace Icons
