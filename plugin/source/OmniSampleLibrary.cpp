#include "OmniDrums/OmniSampleLibrary.h"
#include "DefautSampleData.h"
#include "OmniDrums/Identifiers.h"
#include "juce_audio_formats/juce_audio_formats.h"
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

}  // namespace FactorySamples

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
