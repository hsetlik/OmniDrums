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

class SearchHeader : public Component, public juce::TextEditor::Listener {
private:
  class ColumnHeadBtn : public juce::Button {
  public:
    ColumnHeadBtn(const String& txt);
    void paintButton(juce::Graphics& g, bool highlighted, bool down) override;

  private:
    const String columnText;
  };
  //------------------------------------
  LibSearchState searchState;

  ColumnHeadBtn nameBtn;
  ColumnHeadBtn dateBtn;
  ColumnHeadBtn lengthBtn;
  LibrarySearchBar searchBar;

public:
  struct Listener {
  public:
    Listener() = default;
    virtual ~Listener() {}
    virtual void searchStateChanged(const LibSearchState& state) = 0;
  };
  void addListener(Listener* l);
  void removeListener(Listener* l);
  //------------------------------------
  SearchHeader();
  ~SearchHeader() override;
  void resized() override;
  void paint(juce::Graphics& g) override;
  void textEditorTextChanged(juce::TextEditor& te) override;

private:
  void notifyListeners();
  std::vector<Listener*> listeners;
};
