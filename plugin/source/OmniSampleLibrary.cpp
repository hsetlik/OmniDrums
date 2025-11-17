#include "OmniDrums/OmniSampleLibrary.h"
#include "DefautSampleData.h"
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
static void initFactorySamples(juce::File libFolder) {
  std::vector<juce::File> sampleFiles = {};
  // 1. create all the child files in enum order
  auto fClosedHat = libFolder.getChildFile("HiHats/defaultClosedHat.wav");
  auto result = fClosedHat.create();
  jassert(result.wasOk());
  sampleFiles.push_back(fClosedHat);

  auto fOpenHat = libFolder.getChildFile("HiHats/defaultOpenHat.wav");
  result = fOpenHat.create();
  jassert(result.wasOk());
  sampleFiles.push_back(fOpenHat);

  auto fHiTom = libFolder.getChildFile("Toms/defaultHiTom.wav");
  result = fHiTom.create();
  jassert(result.wasOk());
  sampleFiles.push_back(fHiTom);

  auto fMidTom = libFolder.getChildFile("Toms/defaultMidTom.wav");
  result = fMidTom.create();
  jassert(result.wasOk());
  sampleFiles.push_back(fMidTom);

  auto fLowTom = libFolder.getChildFile("Toms/defaultLowTom.wav");
  result = fLowTom.create();
  jassert(result.wasOk());
  sampleFiles.push_back(fLowTom);

  auto fRide = libFolder.getChildFile("Cymbals/defaultRide.wav");
  result = fRide.create();
  jassert(result.wasOk());
  sampleFiles.push_back(fRide);

  auto fSnare = libFolder.getChildFile("Snares/defaultSnare.wav");
  result = fSnare.create();
  jassert(result.wasOk());
  sampleFiles.push_back(fSnare);

  auto fKick = libFolder.getChildFile("Kicks/defaultKick.wav");
  result = fKick.create();
  jassert(result.wasOk());
  sampleFiles.push_back(fKick);

  // 2. now copy the appropriate binary data into each file
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
    FactorySamples::initFactorySamples(libFolder);
  }
  return libFolder;
}
