#pragma once
#include "../../Common.h"
#include "IconData.h"
#include "AssetData.h"
#include "OmniDrums/GUI/Shared/Color.h"
#include "OmniDrums/Identifiers.h"

namespace Assets {

enum AssetE {
  GradBar,
  KnobBkgnd,
  KnobTop,
  ThumbOn,
  ThumbOff,
  DrumBkgnd,
  UpperBarBkgnd,
  VuMeterOn,
  VuMeterOff,
  DrumPadOn,
  DrumPadOff
};
juce::Image& getImage(const AssetE& id);
}  // namespace Assets

//----------------------------------------

namespace Icons {
enum IconE {
  Kick,
  Snare,
  OpenHat,
  ClosedHat,
  Ride,
  Tom,
  Crash,
  Clap,
  Percussion,
  LibOpen,
  LibClosed,
  Close,
  Search,
  DropdownOpen,
  DropdownClosed,
  PlaybackOn,
  PlaybackOff
};
juce::Image& getImage(const IconE& id);
juce::Image& getDrumIconWithColor(const DrumCategE& id, const Color& color);
void cleanupImages();
}  // namespace Icons
