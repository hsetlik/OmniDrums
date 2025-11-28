//===================================================
#include "OmniDrums/GUI/Shared/Images.h"
#include "AssetData.h"
#include "IconData.h"
#include "OmniDrums/Identifiers.h"

namespace Assets {
juce::Image getImage(const AssetE& id) {
  static juce::Image iGradBar = juce::ImageFileFormat::loadFrom(
      AssetData::GradBar_6x1800_png, AssetData::GradBar_6x1800_pngSize);
  static juce::Image iKnobBkgnd = juce::ImageFileFormat::loadFrom(
      AssetData::KnobBkgnd_35x35_png, AssetData::KnobBkgnd_35x35_pngSize);
  static juce::Image iKnobTop = juce::ImageFileFormat::loadFrom(
      AssetData::KnobTop_72x72_png, AssetData::KnobTop_72x72_pngSize);
  static juce::Image iVertThumbOff = juce::ImageFileFormat::loadFrom(
      AssetData::VertThumbOff_28x42_png, AssetData::VertThumbOff_28x42_pngSize);
  static juce::Image iVertThumbOn = juce::ImageFileFormat::loadFrom(
      AssetData::VertThumbOn_28x42_png, AssetData::VertThumbOn_28x42_pngSize);
  static juce::Image iDrumBkgnd =
      juce::ImageFileFormat::loadFrom(AssetData::DrumIconBkgnd_60x62_png,
                                      AssetData::DrumIconBkgnd_60x62_pngSize);
  static juce::Image iUpperBarBkgnd =
      juce::ImageFileFormat::loadFrom(AssetData::UpperBarBkgnd_296x84_png,
                                      AssetData::UpperBarBkgnd_296x84_pngSize);
  static juce::Image iVuMeter = juce::ImageFileFormat::loadFrom(
      AssetData::VuMeter_28x240_png, AssetData::VuMeter_28x240_pngSize);
  static juce::Image iDrumPadOff = juce::ImageFileFormat::loadFrom(
      AssetData::DrumPadOff_170x170_png, AssetData::DrumPadOff_170x170_pngSize);
  static juce::Image iDrumPadOn = juce::ImageFileFormat::loadFrom(
      AssetData::DrumPadOn_170x170_png, AssetData::DrumPadOn_170x170_pngSize);
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
    case ThumbOff:
      return iVertThumbOff;
      break;
    case ThumbOn:
      return iVertThumbOn;
      break;
    case DrumBkgnd:
      return iDrumBkgnd;
      break;
    case UpperBarBkgnd:
      return iUpperBarBkgnd;
      break;
    case VuMeter:
      return iVuMeter;
      break;
    case DrumPadOff:
      return iDrumPadOff;
      break;
    case DrumPadOn:
      return iDrumPadOn;
      break;
    default:

      jassert(false);
      return iKnobTop;
      break;
  }
}
}  // namespace Assets

namespace Icons {
juce::Image& getImage(const IconE& id) {
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
  static juce::Image iCrash = juce::ImageFileFormat::loadFrom(
      IconData::Crash_256_png, IconData::Crash_256_pngSize);
  static juce::Image iClap = juce::ImageFileFormat::loadFrom(
      IconData::Clap_256_png, IconData::Clap_256_pngSize);
  static juce::Image iPercussion = juce::ImageFileFormat::loadFrom(
      IconData::Percussion_256_png, IconData::Percussion_256_pngSize);
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
      break;
    case Crash:
      return iCrash;
      break;
    case Clap:
      return iClap;
      break;
    case Percussion:
      return iPercussion;
      break;
    case LibOpen:
      return iLibOpen;
      break;
    case LibClosed:
      return iLibClosed;
      break;
    default:
      jassert(false);
      return iKick;
      break;
  }
}

static juce::Image& imgForDrumCateg(const DrumCategE& id) {
  switch (id) {
    case kick:
      return getImage(IconE::Kick);
    case snare:
      return getImage(IconE::Snare);
    case hiHat:
      return getImage(IconE::OpenHat);
    case tom:
      return getImage(IconE::Tom);
    case ride:
      return getImage(IconE::Ride);
    case crash:
      return getImage(IconE::Crash);
    case clap:
      return getImage(IconE::Clap);
    case percussion:
      return getImage(IconE::Percussion);
    case other:
      return getImage(IconE::Percussion);
    default:
      jassert(false);
      return getImage(IconE::Kick);
  }
}

static juce::Image getCopyWithOpaqueColor(juce::Image& img,
                                          const Color& color) {
  if (!img.hasAlphaChannel()) {
    img = img.convertedToFormat(juce::Image::ARGB);
  }
  // 1. figure out which pixels we need to make solid
  juce::RectangleList<int> solids;
  img.createSolidAreaMask(solids, 0.5f);

  // 2. create the destination image & fill it appropriately
  juce::Image destImg(juce::Image::ARGB, img.getWidth(), img.getHeight(), true);
  juce::Graphics g(destImg);
  g.setColour(color);
  g.fillRectList(solids);
  return destImg;
}

// generates a 32 bit ID from an RGB color and an icon ID
static uint32_t getDrumIconID(const DrumCategE& categ, const Color& color) {
  uint32_t id = 0x0000;
  uint32_t rMask = (uint32_t)color.getRed();
  uint32_t gMask = (uint32_t)color.getGreen();
  uint32_t bMask = (uint32_t)color.getBlue();
  // bitwise or with each mask shifted appropriately
  id = id | (rMask << 8);
  id = id | (gMask << 16);
  id = id | (bMask << 24);
  return id | ((uint32_t)categ);
}

// hash map to hold our images
static std::unordered_map<uint32_t, juce::Image> drumIconMap = {};

//
juce::Image& getDrumIconWithColor(const DrumCategE& id, const Color& color) {
  auto iconHash = getDrumIconID(id, color);
  if (!drumIconMap.contains(iconHash)) {
    auto blackImg = imgForDrumCateg(id);
    drumIconMap[iconHash] = getCopyWithOpaqueColor(blackImg, color);
  }
  return drumIconMap[iconHash];
}

void cleanupImages() {
  drumIconMap.clear();
}

}  // namespace Icons
