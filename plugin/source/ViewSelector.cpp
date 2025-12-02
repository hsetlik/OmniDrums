#include "OmniDrums/GUI/ViewSelector.h"
#include "OmniDrums/GUI/Shared/Color.h"
#include "OmniDrums/GUI/Shared/Fonts.h"
#include "OmniDrums/GUI/Shared/Images.h"
#include "juce_core/juce_core.h"
#include "juce_events/juce_events.h"
#include "juce_graphics/juce_graphics.h"

ViewRadioButton::ViewRadioButton(const String& str)
    : juce::ToggleButton(str), btnText(str) {
  // setClickingTogglesState(true);
}
static const float radButtonWidth = 24.0f;

juce::Path ViewRadioButton::getTextPath(const frect_t fBounds) const {
  juce::GlyphArrangement ga;
  const float xRatio = fBounds.getWidth() / radButtonWidth;
  auto font =
      Fonts::getFont(Fonts::KenyanRegular).withPointHeight(18.0f * xRatio);
  ga.addFittedText(font, btnText, 0.0f, 0.0f, fBounds.getHeight(),
                   fBounds.getWidth(), juce::Justification::centred, 1);
  juce::Path p;
  ga.createPath(p);
  auto pBounds = p.getBounds().toFloat();

  auto transform = juce::AffineTransform()
                       .rotated(juce::MathConstants<float>::halfPi,
                                pBounds.getCentreX(), pBounds.getCentreY())
                       .translated(fBounds.getCentreX() - pBounds.getCentreX(),
                                   fBounds.getCentreY() - pBounds.getCentreY());
  p.applyTransform(transform);
  return p;
}

void ViewRadioButton::paintButton(juce::Graphics& g,
                                  bool isHighlighted,
                                  bool isDown) {
  juce::ignoreUnused(isHighlighted, isDown);
  auto fBounds = getLocalBounds().toFloat();
  const float xRatio = fBounds.getWidth() / radButtonWidth;
  // 1. draw the background and border
  auto parent = findParentComponentOfClass<ViewSelector>();
  jassert(parent != nullptr);
  auto fillColor =
      parent->isSelected(this) ? UIColor::orangeLight : UIColor::borderGray;
  g.setColour(fillColor);
  g.fillRect(fBounds);
  g.setColour(UIColor::bkgndGray);
  g.fillRect(fBounds.reduced(0.8f * xRatio));
  // 2. draw the text
  g.setColour(fillColor);
  auto path = getTextPath(fBounds);
  g.fillPath(path);
}
//===================================================

void LibOpenCloseButton::paintButton(juce::Graphics& g,
                                     bool isHighlighted,
                                     bool isDown) {
  juce::ignoreUnused(isHighlighted, isDown);
  auto img = getToggleState() ? Icons::getImage(Icons::LibOpen)
                              : Icons::getImage(Icons::LibClosed);
  auto fBounds = getLocalBounds().toFloat();
  auto xRatio = fBounds.getWidth() / 24.0f;
  frect_t btnBounds = {0.0f, 0.0f, 24.0f * xRatio, 24.0f * xRatio};
  g.setColour(UIColor::bkgndGray);
  g.fillRect(btnBounds);
  g.drawImage(img, btnBounds.reduced(2.0f * xRatio));
}

//===================================================

ViewSelector::ViewSelector(OmniState* s)
    : state(s),
      sampleBtn("SAMPLES"),
      compressorBtn("COMPRESSOR"),
      roomBtn("ROOM") {
  addAndMakeVisible(sampleBtn);
  sampleBtn.setToggleState(true, juce::dontSendNotification);
  addAndMakeVisible(compressorBtn);
  addAndMakeVisible(roomBtn);
  addAndMakeVisible(libBtn);
  libBtn.setClickingTogglesState(true);

  sampleBtn.onClick = [this]() { setSelected(&sampleBtn); };
  compressorBtn.onClick = [this]() { setSelected(&compressorBtn); };
  roomBtn.onClick = [this]() { setSelected(&roomBtn); };
  libBtn.onClick = [this]() {
    state->setLibOpen(libBtn.getToggleState());
    for (auto* l : listeners)
      l->libStateChanged(libBtn.getToggleState());
  };
}

void ViewSelector::setSelected(ViewRadioButton* btn) {
  if (btn != selectedBtn) {
    selectedBtn = btn;
    repaint();
    ViewE id = vSamples;
    if (btn == &compressorBtn) {
      id = vCompressor;
    } else if (btn == &roomBtn) {
      id = vRoom;
    }
    for (auto* l : listeners) {
      l->viewSelected(id);
    }
  }
}

void ViewSelector::addListener(ViewSelectorListener* ptr) {
  listeners.push_back(ptr);
}

void ViewSelector::removeListener(ViewSelectorListener* ptr) {
  for (auto it = listeners.begin(); it != listeners.end(); ++it) {
    if (*it == ptr) {
      listeners.erase(it);
      return;
    }
  }
}

void ViewSelector::resized() {
  auto fBounds = getLocalBounds().toFloat();
  const float xRatio = fBounds.getWidth() / 24.0f;
  const float yRatio = fBounds.getHeight() / 630.0f;
  frect_t libBounds = {0.0f, 0.0f, 24.0f * xRatio, 24.0f * xRatio};
  float yTop = 24.0f * xRatio;
  frect_t sBounds = {0.0f, yTop, 24.0f * xRatio, 65.0f * yRatio};
  yTop += sBounds.getHeight();
  frect_t cBounds = {0.0f, yTop, 24.0f * xRatio, 82.0f * yRatio};
  yTop += cBounds.getHeight();
  frect_t rBounds = {0.0f, yTop, 24.0f * xRatio, 53.0f * yRatio};
  libBtn.setBounds(libBounds.toNearestInt());
  sampleBtn.setBounds(sBounds.toNearestInt());
  compressorBtn.setBounds(cBounds.toNearestInt());
  roomBtn.setBounds(rBounds.toNearestInt());
}

void ViewSelector::paint(juce::Graphics& g) {
  auto fBounds = getLocalBounds().toFloat();
  g.setColour(UIColor::bkgndGray);
  g.fillRect(fBounds);
}
