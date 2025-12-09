#pragma once
#include "SearchHeader.h"

// component for each sample in the list
class LibEntryComponent : public Component {
  OmniState* const state;
  ValueTree sampleTree;
  String fileName;
  juce::Time timeAdded;
  int lengthMs;
  bool isSelected() const;

public:
  LibEntryComponent(OmniState* s, ValueTree tree);
  void paint(juce::Graphics& g) override;
  String getFileName() const { return fileName; }
  juce::Time getDateAdded() const { return timeAdded; }
  int getLengthMs() const { return lengthMs; }
  bool compare(int sortMode, const LibEntryComponent& other) const;
  void mouseDown(const juce::MouseEvent& e) override;
  void enablementChanged() override;
  juce::File getSampleFile() const;
};

class DropdownButton : public juce::Button {
public:
  DropdownButton();
  void paintButton(juce::Graphics& g, bool down, bool highlighted) override;
};
// // header for each category
class CategoryHeader : public Component {
private:
  OmniState* const state;
  const int categoryID;
  bool listIsOpen = false;
  DropdownButton btn;

public:
  CategoryHeader(OmniState* s, int idx);
  void paint(juce::Graphics& g) override;
  void resized() override;
  bool isOpen() const { return listIsOpen; }
};

//===========================================
class CategoryHolder : public Component, public SearchHeader::Listener {
private:
  OmniState* const state;
  SearchHeader* const searchHeader;
  const int categoryID;
  CategoryHeader header;
  juce::OwnedArray<LibEntryComponent> entries;
  int currentSortMode = 0;
  String currentSearchText = "";
  float xScale = 1.0f;
  float yScale = 1.0f;
  std::vector<LibEntryComponent*> getVisibleEntries() const;
  void sortEntries(std::vector<LibEntryComponent*>& list) const;

public:
  CategoryHolder(OmniState* s, SearchHeader* h, int idx);
  ~CategoryHolder() override;
  void setScales(float x, float y) {
    xScale = x;
    yScale = y;
  }
  int numBarsVisible() const;
  void resized() override;
  void searchStateChanged(const LibSearchState& search) override;
};

//===========================================
class ViewedSampleList : public Component {
private:
  juce::OwnedArray<CategoryHolder> categories;
  float xScale = 1.0f;
  float yScale = 1.0f;

public:
  ViewedSampleList(OmniState* s, SearchHeader* h);
  void setScales(float x, float y);
  void resized() override;
};

//===========================================
class DetailView : public Component {
private:
  struct PlaybackButton : public juce::Button {
  public:
    PlaybackButton();
    void paintButton(juce::Graphics& g, bool down, bool highlighted) override;
  };
  PlaybackButton playBtn;
  juce::Image waveImg;
  LibEntryComponent* entry = nullptr;
  static const int waveWidth = 285;
  juce::AudioFormatManager manager;
  // this is where we draw the waveform
  void drawWaveformImage();

public:
  DetailView();
  void resized() override;
  void paint(juce::Graphics& g) override;
  void setSelectedSample(LibEntryComponent* comp);
};

//===========================================

class SampleBrowser : public Component, public SearchHeader::Listener {
private:
  OmniState* const state;
  SearchHeader header;
  juce::Viewport vpt;
  ViewedSampleList list;
  LibEntryComponent* selectedEntry = nullptr;
  DetailView detailView;

public:
  SampleBrowser(OmniState* s);
  ~SampleBrowser() override;
  void searchStateChanged(const LibSearchState& searchState) override;
  void resized() override;
  bool isSelected(const LibEntryComponent* entry) const {
    return selectedEntry == entry;
  }
  void setSelected(LibEntryComponent* e);
};
