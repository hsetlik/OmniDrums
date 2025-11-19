#pragma once
#include "../../Common.h"
#include "IconData.h"
#include "AssetData.h"

namespace Assets {

enum AssetE {
  GradBar,
  KnobBkgnd,
  KnobTop,
  ThumbOn,
  ThumbOff,
  DrumBkgnd,
  UpperBarBkgnd,
  VuMeter,
  DrumPadOn,
  DrumPadOff
};
juce::Image getImage(const AssetE& id);
}  // namespace Assets

//----------------------------------------

namespace Icons {
enum IconE { Kick, Snare, OpenHat, ClosedHat, Ride, Tom, LibOpen, LibClosed };
juce::Image getImage(const IconE& id);
}  // namespace Icons
