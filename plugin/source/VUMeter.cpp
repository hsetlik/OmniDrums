#include "OmniDrums/GUI/SampleView/VUMeter.h"
#include "OmniDrums/GUI/Shared/Images.h"
#include "OmniDrums/Identifiers.h"
#include "juce_audio_basics/juce_audio_basics.h"
//===================================================

ChannelVUMeter::ChannelVUMeter(OmniState* s, int idx)
    : state(s), channelIdx(idx) {
  state->graphingData.registerListener(this);
}

ChannelVUMeter::~ChannelVUMeter() {
  state->graphingData.deregisterListener(this);
}

static frange_t lvlRange =
    rangeWithCenter(0.0f, 1.0f, juce::Decibels::decibelsToGain(-12.0f));

int ChannelVUMeter::bottomPxForLeft() const {
  float rmsLvl = state->graphingData.getLeftChannelLevel(channelIdx);
  rmsLvl = lvlRange.getRange().clipValue(rmsLvl);
  int onImgHeight =
      (int)(lvlRange.convertTo0to1(rmsLvl) * (float)VU_METER_HEIGHT);
  return VU_METER_HEIGHT - onImgHeight;
}

int ChannelVUMeter::bottomPxForRight() const {
  float rmsLvl = state->graphingData.getRightChannelLevel(channelIdx);
  rmsLvl = lvlRange.getRange().clipValue(rmsLvl);
  int onImgHeight =
      (int)(lvlRange.convertTo0to1(rmsLvl) * (float)VU_METER_HEIGHT);
  return VU_METER_HEIGHT - onImgHeight;
}

static const float destWidthF = 35.0f;
static const float imgDestWidth = 15.0f;

void ChannelVUMeter::paint(juce::Graphics& g) {
  auto fBounds = getLocalBounds().toFloat();
  const float xScale = fBounds.getWidth() / destWidthF;
  // draw the left meter
  auto fLeftBounds = fBounds.removeFromLeft(imgDestWidth * xScale);
  drawMeterAt(g, fLeftBounds, currentLeftBottom);
  // draw right meter
  auto fRightBounds = fBounds.removeFromRight(imgDestWidth * xScale);
  drawMeterAt(g, fRightBounds, currentRightBottom);
}

void ChannelVUMeter::drawMeterAt(juce::Graphics& g,
                                 const frect_t& bounds,
                                 int srcBottomPx) const {
  auto& offImg = Assets::getImage(Assets::VuMeterOff);
  auto& onImg = Assets::getImage(Assets::VuMeterOn);
  // 1. find float rectangles for the destination side off the image
  const float bottomNorm = (float)srcBottomPx / (float)VU_METER_HEIGHT;
  frect_t onDestBounds = bounds;
  frect_t offDestBounds =
      onDestBounds.removeFromTop(onDestBounds.getHeight() * bottomNorm);
  // 2. convert to int
  auto offDestI = offDestBounds.toNearestInt();
  auto onDestI = onDestBounds.toNearestInt();
  // 3. draw the off (upper) image
  g.drawImage(offImg, offDestI.getX(), offDestI.getY(), offDestI.getWidth(),
              offDestI.getHeight(), 0, 0, VU_METER_WIDTH, srcBottomPx);

  // 4. draw the on (lower) image
  g.drawImage(onImg, onDestI.getX(), onDestI.getY(), onDestI.getWidth(),
              onDestI.getHeight(), 0, srcBottomPx, VU_METER_WIDTH,
              VU_METER_HEIGHT - srcBottomPx);
}

void ChannelVUMeter::graphingDataUpdated() {
  auto newLeftBottom = bottomPxForLeft();
  auto newRightBottom = bottomPxForRight();
  if (newLeftBottom != currentLeftBottom ||
      newRightBottom != currentRightBottom) {
    currentLeftBottom = newLeftBottom;
    currentRightBottom = newRightBottom;
    repaint();
  }
}
