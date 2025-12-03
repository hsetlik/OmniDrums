#pragma once
#include "../../OmniState.h"
#include "../../Audio/Core/SampleCache.h"
#include "OmniDrums/Identifiers.h"
#include "juce_gui_basics/juce_gui_basics.h"

enum SortModeE { sName, sDateAdded, sLength };

struct LibSearchState {
  int sortMode = 0;
  String searchText = "";
};

//==================================================================

class SearchBarLnF : public juce::LookAndFeel_V4 {
public:
  SearchBarLnF();
  void fillTextEditorBackground(juce::Graphics& g,
                                int width,
                                int height,
                                juce::TextEditor& te) override;
  void drawTextEditorOutline(juce::Graphics& g,
                             int width,
                             int height,
                             juce::TextEditor& te) override;
};

class ClearButton : public juce::Button {
public:
  ClearButton() : juce::Button("clearBar") {}
  void paintButton(juce::Graphics& g, bool highlighted, bool down) override;
};

class LibrarySearchBar : public Component {
public:
  ClearButton clearBtn;
  juce::TextEditor editor;
  LibrarySearchBar();
  ~LibrarySearchBar() override;
  void resized() override;
  void paint(juce::Graphics& g) override;

private:
  SearchBarLnF lnf;
};

//==================================================================

class SearchHeader : public Component {
public:
  struct Listener {
  public:
    Listener() = default;
    virtual ~Listener() {}
    virtual void searchStateChanged(const LibSearchState& state) = 0;
  };
};
