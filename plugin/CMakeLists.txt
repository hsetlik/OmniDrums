cmake_minimum_required(VERSION 3.22)
# Version is needed by JUCE.
project(OmniDrums VERSION 0.1.0)
# Adding a directory with the library/application name as a subfolder of the
# include folder is a good practice. It helps avoid name clashes later on.
set(INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include/OmniDrums")

# add binary data for the icons
set(ICONS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/icons")
set(ICON_FILES
"${ICONS_DIR}/ClosedHat_256.png"
"${ICONS_DIR}/Kick_256.png"
"${ICONS_DIR}/OpenHat_256.png"
"${ICONS_DIR}/Ride_256.png"
"${ICONS_DIR}/Snare_256.png"
"${ICONS_DIR}/Tom_256.png"
"${ICONS_DIR}/LibOpen_256.png"
"${ICONS_DIR}/LibClosed_256.png"
"${ICONS_DIR}/Crash_256.png"
"${ICONS_DIR}/Clap_256.png"
"${ICONS_DIR}/Percussion_256.png"
"${ICONS_DIR}/Search_256.png"
"${ICONS_DIR}/Close_256.png"
)


juce_add_binary_data(
  iconBinary
  HEADER_NAME
  "IconData.h"
  NAMESPACE
  "IconData"
  SOURCES
  "${ICON_FILES}")


# binary data for other png assets
set(ASSETS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/assets")
set(ASSET_FILES
"${ASSETS_DIR}/GradBar_6x1800.png"
"${ASSETS_DIR}/KnobBkgnd_35x35.png"
"${ASSETS_DIR}/KnobTop_24x24.png"
"${ASSETS_DIR}/KnobTop_72x72.png"
"${ASSETS_DIR}/VertThumbOff_28x42.png"
"${ASSETS_DIR}/VertThumbOn_28x42.png"
"${ASSETS_DIR}/DrumIconBkgnd_60x62.png"
"${ASSETS_DIR}/UpperBarBkgnd_296x84.png"
"${ASSETS_DIR}/VuMeterOn_28x240.png"
"${ASSETS_DIR}/VuMeterOff_28x240.png"
"${ASSETS_DIR}/DrumPadOff_170x170.png"
"${ASSETS_DIR}/DrumPadOn_170x170.png"
)

juce_add_binary_data(
  assetBinary
  HEADER_NAME
  "AssetData.h"
  NAMESPACE
  "AssetData"
  SOURCES
  "${ASSET_FILES}"
)

# binary data for defaut samples

set(FACTORY_SAMPLES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/factory_samples")
set(FACTORY_SAMPLE_FILES
"${FACTORY_SAMPLES_DIR}/defaultClosedHat.wav"
"${FACTORY_SAMPLES_DIR}/defaultHiTom.wav"
"${FACTORY_SAMPLES_DIR}/defaultKick.wav"
"${FACTORY_SAMPLES_DIR}/defaultLowTom.wav"
"${FACTORY_SAMPLES_DIR}/defaultMidTom.wav"
"${FACTORY_SAMPLES_DIR}/defaultOpenHat.wav"
"${FACTORY_SAMPLES_DIR}/defaultRide.wav"
"${FACTORY_SAMPLES_DIR}/defaultSnare.wav"
)

juce_add_binary_data(
  factorySamplesBinary
  HEADER_NAME
  "DefautSampleData.h"
  NAMESPACE
  "DefaultSampleData"
  SOURCES
  "${FACTORY_SAMPLE_FILES}"
)

# binary data for font files
set(FONTS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/fonts")
set(FONT_FILES
"${FONTS_DIR}/KenyanCoffeeBoldItalic.otf"
"${FONTS_DIR}/KenyanCoffeeRegular.otf"
"${FONTS_DIR}/RobotoLightItalic.ttf"
"${FONTS_DIR}/RobotoMediumItalic.ttf"
"${FONTS_DIR}/RobotoLight.ttf"
)

juce_add_binary_data(
  fontBinary
  HEADER_NAME
  "FontData.h"
  NAMESPACE
  "FontData"
  SOURCES
  "${FONT_FILES}"
)

# Adds a plugin target (that's basically what the Projucer does).
juce_add_plugin(${PROJECT_NAME}
    COMPANY_NAME Setlik # change this
    IS_SYNTH true # may change this
    NEEDS_MIDI_INPUT true # may change this
    NEEDS_MIDI_OUTPUT FALSE # may change this
    PLUGIN_MANUFACTURER_CODE STLK # change this
    PLUGIN_CODE OMND # change this
    FORMATS Standalone VST3  AU # may change this
    PRODUCT_NAME "OmniDrums" # change this
    COPY_PLUGIN_AFTER_BUILD TRUE 
)

# Sets the source files of the plugin project.
target_sources(${PROJECT_NAME}
    PRIVATE
        source/Identifiers.cpp
        source/Common.cpp
        source/PluginEditor.cpp
        source/PluginProcessor.cpp
        ${INCLUDE_DIR}/Identifiers.h
        ${INCLUDE_DIR}/Common.h
        ${INCLUDE_DIR}/PluginEditor.h
				source/Images.cpp
				${INCLUDE_DIR}/GUI/Shared/Images.h
				source/Fonts.cpp
				${INCLUDE_DIR}/GUI/Shared/Fonts.h
				${INCLUDE_DIR}/GUI/Shared/Color.h
				source/OmniLookAndFeel.cpp
				${INCLUDE_DIR}/GUI/Shared/OmniLookAndFeel.h
				source/OmniState.cpp
				${INCLUDE_DIR}/OmniState.h
				source/OmniEditor.cpp
				${INCLUDE_DIR}/GUI/OmniEditor.h
				source/OmniSampleLibrary.cpp
				${INCLUDE_DIR}/OmniSampleLibrary.h
				source/SamplePlayer.cpp
				${INCLUDE_DIR}/Audio/SamplePlayer.h
				source/ChannelComponent.cpp
				source/SampleView.cpp
				${INCLUDE_DIR}/GUI/SampleView/ChannelComponent.h
				source/DrumChannel.cpp
				${INCLUDE_DIR}/Audio/Core/DrumChannel.h
				source/SampleCache.cpp
				${INCLUDE_DIR}/Audio/Core/SampleCache.h
				source/OmniEngine.cpp
				${INCLUDE_DIR}/Audio/Core/OmniEngine.h
				source/Util.cpp
				${INCLUDE_DIR}/GUI/Shared/Util.h
				${INCLUDE_DIR}/GUI/SampleView/SampleView.h
				source/RollingRMS.cpp
				${INCLUDE_DIR}/Audio/Shared/RollingRMS.h
				source/VUMeter.cpp
				${INCLUDE_DIR}/GUI/SampleView/VUMeter.h
				source/ViewSelector.cpp
				${INCLUDE_DIR}/GUI/ViewSelector.h
				source/SearchHeader.cpp
				${INCLUDE_DIR}/GUI/Browser/SearchHeader.h
        source/SampleList.cpp
        ${INCLUDE_DIR}/GUI/Browser/SampleList.h
        ${INCLUDE_DIR}/PluginProcessor.h
)

# Sets the include directories of the plugin project.
target_include_directories(${PROJECT_NAME}
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)


# Links to all necessary dependencies. The present ones are recommended by JUCE.
target_link_libraries(${PROJECT_NAME}
    PRIVATE
        juce::juce_audio_utils
    PUBLIC
        iconBinary
        assetBinary
        fontBinary
        factorySamplesBinary
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags
        juce::juce_dsp
)


# These definitions are recommended by JUCE.
target_compile_definitions(${PROJECT_NAME}
    PUBLIC
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_VST3_CAN_REPLACE_VST2=0
)
# Enables all warnings and treats warnings as errors.
# This needs to be set up only for your projects, not 3rd party
if (MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4 /WX)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
endif()

# In Visual Studio this command provides a nice grouping of source files in "filters".
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/..)

