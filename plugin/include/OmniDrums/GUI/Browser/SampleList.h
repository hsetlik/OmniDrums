#pragma once
#include "SearchHeader.h"

// component for each sample in the list
class LibEntryComponent : public Component {
  OmniState* const state;
  ValueTree sampleTree;
  String fileName;
  juce::Time timeAdded;
  int lengthMs;

public:
  LibEntryComponent(OmniState* s, ValueTree tree);
  void paint(juce::Graphics& g) override;
  String getFileName() const { return fileName; }
  juce::Time getDateAdded() const { return timeAdded; }
  int getLengthMs() const { return lengthMs; }
  bool compare(int sortMode, const LibEntryComponent& other) const;
};

class DropdownButton : public juce::Button {
public:
  DropdownButton();
  void paintButton(juce::Graphics& g, bool down, bool highlighted);
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
  bool isOpen() const { return listIsOpen; }
};

//===========================================

class SampleBrowser : public Component, public SearchHeader::Listener {
private:
  OmniState* const state;
  SearchHeader header;

public:
  SampleBrowser(OmniState* s);
  ~SampleBrowser() override;
  void searchStateChanged(const LibSearchState& searchState) override;
  void resized() override;
};
