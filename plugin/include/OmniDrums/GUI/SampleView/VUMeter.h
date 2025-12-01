#pragma once
#include "../Shared/Images.h"
#include "OmniDrums/OmniState.h"

#define VU_METER_HEIGHT 240
#define VU_METER_WIDTH 28

class ChannelVUMeter : public Component, public GraphingData::Listener {
private:
  OmniState* const state;
  const int channelIdx;
  int bottomPxForLeft() const;
  int bottomPxForRight() const;
  void drawMeterAt(juce::Graphics& g,
                   const frect_t& bounds,
                   int srcBottomPx) const;
  int currentLeftBottom = 0;
  int currentRightBottom = 0;

public:
  ChannelVUMeter(OmniState* state, int idx);
  ~ChannelVUMeter() override;
  void graphingDataUpdated() override;
  void paint(juce::Graphics& g) override;
};

