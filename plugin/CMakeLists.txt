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
)

juce_add_binary_data(
  iconBinary
  HEADER_NAME
  "IconData.h"
  NAMESPACE
  "IconData"
  SOURCES
  "${ICON_FILES}")


# Adds a plugin target (that's basically what the Projucer does).
juce_add_plugin(${PROJECT_NAME}
    COMPANY_NAME Setlik # change this
    IS_SYNTH true # may change this
    NEEDS_MIDI_INPUT FALSE # may change this
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

