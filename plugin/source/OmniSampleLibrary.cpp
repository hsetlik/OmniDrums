#include "OmniDrums/OmniSampleLibrary.h"
#include "DefautSampleData.h"
#include "OmniDrums/Audio/SamplePlayer.h"
#include "OmniDrums/Identifiers.h"
#include "juce_audio_formats/juce_audio_formats.h"
#include "juce_core/juce_core.h"
#include "juce_core/system/juce_PlatformDefs.h"
namespace FactorySamples {
static void writeBinaryToFile(const SampleE& id, juce::File& dest) {
  switch (id) {
    case closedHat:
      jassert(
          dest.replaceWithData(DefaultSampleData::defaultClosedHat_wav,
                               DefaultSampleData::defaultClosedHat_wavSize));
      break;
    case openHat:
      jassert(dest.replaceWithData(DefaultSampleData::defaultOpenHat_wav,
                                   DefaultSampleData::defaultOpenHat_wavSize));
      break;
    case hiTom:
      jassert(dest.replaceWithData(DefaultSampleData::defaultHiTom_wav,
                                   DefaultSampleData::defaultHiTom_wavSize));
      break;
    case midTom:
      jassert(dest.replaceWithData(DefaultSampleData::defaultMidTom_wav,
                                   DefaultSampleData::defaultMidTom_wavSize));

      break;
    case lowTom:
      jassert(dest.replaceWithData(DefaultSampleData::defaultLowTom_wav,
                                   DefaultSampleData::defaultLowTom_wavSize));
      break;
    case ride:
      jassert(dest.replaceWithData(DefaultSampleData::defaultRide_wav,
                                   DefaultSampleData::defaultRide_wavSize));
      break;
    case snare:
      jassert(dest.replaceWithData(DefaultSampleData::defaultSnare_wav,
                                   DefaultSampleData::defaultSnare_wavSize));
      break;
    case kick:
      jassert(dest.replaceWithData(DefaultSampleData::defaultKick_wav,
                                   DefaultSampleData::defaultKick_wavSize));
      break;
    default:
      jassert(false);
      break;
  }
}

juce::File getSampleFile(const SampleE& id, const juce::File& libFolder) {
  switch (id) {
    case closedHat:
      return libFolder.getChildFile(stringFor(DrumCategE::hiHat) +
                                    "/defaultClosedHat.wav");
      break;
    case openHat:
      return libFolder.getChildFile(stringFor(DrumCategE::hiHat) +
                                    "/defaultOpenHat.wav");
      break;
    case hiTom:
      return libFolder.getChildFile(stringFor(DrumCategE::tom) +
                                    "/defaultHiTom.wav");
      break;
    case midTom:
      return libFolder.getChildFile(stringFor(DrumCategE::tom) +
                                    "/defaultMidTom.wav");
      break;
    case lowTom:
      return libFolder.getChildFile(stringFor(DrumCategE::tom) +
                                    "/defaultLowTom.wav");
      break;
    case ride:
      return libFolder.getChildFile(stringFor(DrumCategE::ride) +
                                    "/defaultRide.wav");
      break;
    case snare:
      return libFolder.getChildFile(stringFor(DrumCategE::snare) +
                                    "/defaultSnare.wav");
      break;
    case kick:
      return libFolder.getChildFile(stringFor(DrumCategE::kick) +
                                    "/defaultKick.wav");
      break;
  }
  jassert(false);
  return juce::File();
}

static void initFactorySamples(juce::File libFolder) {
  // 1. create child folders for each drum category
  for (int i = 0; i < NUM_DRUM_CATEGORIES; ++i) {
    auto folder = libFolder.getChildFile(drumCategoryNames[i]);
    auto dirResult = folder.createDirectory();
    jassert(dirResult.wasOk());
  }

  // 2. create all the child files in enum order
  std::vector<juce::File> sampleFiles = {};
  auto fClosedHat = getSampleFile(SampleE::closedHat, libFolder);
  auto result = fClosedHat.create();
  jassert(result.wasOk());
  sampleFiles.push_back(fClosedHat);

  auto fOpenHat = getSampleFile(SampleE::openHat, libFolder);
  result = fOpenHat.create();
  jassert(result.wasOk());
  sampleFiles.push_back(fOpenHat);

  auto fHiTom = getSampleFile(SampleE::hiTom, libFolder);
  result = fHiTom.create();
  jassert(result.wasOk());
  sampleFiles.push_back(fHiTom);

  auto fMidTom = getSampleFile(SampleE::midTom, libFolder);
  result = fMidTom.create();
  jassert(result.wasOk());
  sampleFiles.push_back(fMidTom);

  auto fLowTom = getSampleFile(SampleE::lowTom, libFolder);
  result = fLowTom.create();
  jassert(result.wasOk());
  sampleFiles.push_back(fLowTom);

  auto fRide = getSampleFile(SampleE::ride, libFolder);
  result = fRide.create();
  jassert(result.wasOk());
  sampleFiles.push_back(fRide);

  auto fSnare = getSampleFile(SampleE::snare, libFolder);
  result = fSnare.create();
  jassert(result.wasOk());
  sampleFiles.push_back(fSnare);

  auto fKick = getSampleFile(SampleE::kick, libFolder);
  result = fKick.create();
  jassert(result.wasOk());
  sampleFiles.push_back(fKick);

  // 3. now copy the appropriate binary data into each file
  for (size_t i = 0; i < sampleFiles.size(); ++i) {
    writeBinaryToFile((SampleE)i, sampleFiles[i]);
  }
}

ValueTree getDefaultSampleState(const SampleE& id) {
  ValueTree vt(ID::OMNI_PLAYER_SAMPLE);
  switch (id) {
    case closedHat:
      vt.setProperty(ID::sampleDrumCategory, (int)DrumCategE::hiHat, nullptr);
      vt.setProperty(ID::sampleFileName, "defaultClosedHat.wav", nullptr);
      vt.setProperty(ID::sampleDrumChannel, 7, nullptr);
      break;
    case openHat:
      vt.setProperty(ID::sampleDrumCategory, (int)DrumCategE::hiHat, nullptr);
      vt.setProperty(ID::sampleFileName, "defaultOpenHat.wav", nullptr);
      vt.setProperty(ID::sampleDrumChannel, 11, nullptr);
      break;
    case hiTom:
      vt.setProperty(ID::sampleDrumCategory, (int)DrumCategE::tom, nullptr);
      vt.setProperty(ID::sampleFileName, "defaultHiTom.wav", nullptr);
      vt.setProperty(ID::sampleDrumChannel, 15, nullptr);
      break;
    case midTom:
      vt.setProperty(ID::sampleDrumCategory, (int)DrumCategE::tom, nullptr);
      vt.setProperty(ID::sampleFileName, "defaultMidTom.wav", nullptr);
      vt.setProperty(ID::sampleDrumChannel, 12, nullptr);
      break;
    case lowTom:
      vt.setProperty(ID::sampleDrumCategory, (int)DrumCategE::tom, nullptr);
      vt.setProperty(ID::sampleFileName, "defaultLowTom.wav", nullptr);
      vt.setProperty(ID::sampleDrumChannel, 10, nullptr);
      break;
    case ride:
      vt.setProperty(ID::sampleDrumCategory, (int)DrumCategE::ride, nullptr);
      vt.setProperty(ID::sampleFileName, "defaultRide.wav", nullptr);
      vt.setProperty(ID::sampleDrumChannel, 16, nullptr);
      break;
    case snare:
      vt.setProperty(ID::sampleDrumCategory, (int)DrumCategE::snare, nullptr);
      vt.setProperty(ID::sampleFileName, "defaultSnare.wav", nullptr);
      vt.setProperty(ID::sampleDrumChannel, 3, nullptr);
      break;
    case kick:
      vt.setProperty(ID::sampleDrumCategory, (int)DrumCategE::kick, nullptr);
      vt.setProperty(ID::sampleFileName, "defaultKick.wav", nullptr);
      vt.setProperty(ID::sampleDrumChannel, 0, nullptr);
      break;
    default:
      jassert(false);
      break;
  }
  return vt;
}

}  // namespace FactorySamples

//===================================================
//===================================================

juce::File OmniSampleLibrary::getSampleLibFolder() {
  auto appData =
      juce::File::getSpecialLocation(juce::File::userApplicationDataDirectory);
  jassert(appData.exists() && appData.isDirectory());
  auto libFolder = appData.getChildFile("OmniDrumsSamples");
  if (!libFolder.exists() || !libFolder.isDirectory()) {
    auto result = libFolder.createDirectory();
    jassert(result.wasOk());
    FactorySamples::initFactorySamples(libFolder);
  }
  return libFolder;
}

static ValueTree buildLibChildTree(int categID,
                                   const String& fileName,
                                   juce::AudioFormatManager* manager) {
  ValueTree vt(ID::SAMPLE_LIB_ENTRY);
  vt.setProperty(ID::sampleFileName, fileName, nullptr);
  vt.setProperty(ID::sampleDrumCategory, categID, nullptr);
  auto now = juce::Time::getCurrentTime();
  vt.setProperty(ID::libSampleDateAdded, now.toISO8601(true), nullptr);
  auto file = OmniSampleLibrary::getSampleLibFolder().getChildFile(
      drumCategoryNames[categID] + "/" + fileName);
  jassert(file.existsAsFile());
  auto reader = juce::rawToUniquePtr(manager->createReaderFor(file));
  jassert(reader != nullptr);
  auto lengthSeconds = (double)reader->lengthInSamples / reader->sampleRate;
  const int lengthMs = (int)(lengthSeconds * 1000.0f);
  vt.setProperty(ID::libSampleLengthMs, lengthMs, nullptr);
  return vt;
}

ValueTree OmniSampleLibrary::buildDefaultLibrary() {
  ValueTree vt(ID::OMNI_SAMPLE_LIB);
  for (int i = 0; i < NUM_DRUM_CATEGORIES; ++i) {
    ValueTree categTree(ID::SAMPLE_LIB_CATEGORY);
    categTree.setProperty(ID::sampleCategoryIndex, i, nullptr);
    categTree.setProperty(ID::sampleCategoryName, drumCategoryNames[i],
                          nullptr);
    auto folder = getSampleLibFolder().getChildFile(drumCategoryNames[i]);
    auto files = folder.findChildFiles(juce::File::findFiles, true);

    for (int f = 0; f < files.size(); ++f) {
      if (!files[f].isHidden()) {
        auto name = files[f].getRelativePathFrom(folder);
        auto childTree = buildLibChildTree(i, name, this->manager);
        categTree.appendChild(childTree, nullptr);
      }
    }
    vt.appendChild(categTree, nullptr);
  }
  return vt;
}

juce::File OmniSampleLibrary::getLibraryDataFile() {
  auto appData =
      juce::File::getSpecialLocation(juce::File::userApplicationDataDirectory);
  jassert(appData.exists() && appData.isDirectory());
  auto libFile = appData.getChildFile("SampleLibraryData.odlib");
  if (!libFile.existsAsFile()) {
    auto result = libFile.create();
    jassert(result.wasOk());
    // now create a tree for whatever samples exist and write it to the file
    auto vt = buildDefaultLibrary();
    auto libString = vt.toXmlString();
    jassert(libFile.replaceWithText(libString));
  }
  return libFile;
}

static ValueTree loadFromXmlFile(const juce::File& libFile) {
  auto xmlStr = libFile.loadFileAsString();
  return ValueTree::fromXml(xmlStr);
}

OmniSampleLibrary::OmniSampleLibrary(juce::AudioFormatManager* mngr)
    : manager(mngr),
      libFolder(getSampleLibFolder()),
      sampleLibState(loadFromXmlFile(getLibraryDataFile())) {
  jassert(libFolder.exists() && libFolder.isDirectory());
  recordNewSamples();
}

OmniSampleLibrary::~OmniSampleLibrary() {
  auto xmlStr = sampleLibState.toXmlString();
  auto file = getLibraryDataFile();
  jassert(file.replaceWithText(xmlStr));
}

bool OmniSampleLibrary::recordedInLibState(int drumCateg,
                                           const String& fileName) const {
  auto categTree =
      sampleLibState.getChildWithProperty(ID::sampleCategoryIndex, drumCateg);
  if (!categTree.isValid() || !categTree.hasType(ID::SAMPLE_LIB_CATEGORY)) {
    jassert(false);
    return false;
  }
  auto sampleTree =
      categTree.getChildWithProperty(ID::sampleFileName, fileName);
  return sampleTree.isValid();
}

void OmniSampleLibrary::recordNewSamples() {
  for (int c = 0; c < NUM_DRUM_CATEGORIES; ++c) {
    auto folder = getSampleLibFolder().getChildFile(drumCategoryNames[c]);
    auto files = folder.findChildFiles(juce::File::findFiles, true);
    auto categTree =
        sampleLibState.getChildWithProperty(ID::sampleCategoryIndex, c);
    if (files.size() != categTree.getNumChildren()) {
      for (int f = 0; f < files.size(); ++f) {
        if (!files[f].isHidden()) {
          auto name = files[f].getFileName();
          if (!recordedInLibState(c, name)) {
            categTree.appendChild(buildLibChildTree(c, name, manager), nullptr);
          }
        }
      }
    }
  }
}

juce::File OmniSampleLibrary::fileForSample(
    const ValueTree& sampleState) const {
  jassert(sampleState.hasType(ID::OMNI_PLAYER_SAMPLE) ||
          sampleState.hasType(ID::SAMPLE_LIB_ENTRY));
  const int iCategory = sampleState[ID::sampleDrumCategory];
  const String fileName = sampleState[ID::sampleFileName];
  auto file =
      libFolder.getChildFile(drumCategoryNames[iCategory] + "/" + fileName);
  jassert(file.existsAsFile());
  return file;
}

ValueTree OmniSampleLibrary::getCategoryTree(int idx) {
  auto tree = sampleLibState.getChildWithProperty(ID::sampleCategoryIndex, idx);
  jassert(tree.isValid());
  return tree;
}
