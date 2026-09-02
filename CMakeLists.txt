cmake_minimum_required(VERSION 3.22)

project(SpectralGate VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 23)

# JUCE
add_subdirectory(JUCE)

# Plugin
juce_add_plugin(SpectralGate
    COMPANY_NAME "ArchieHodgson"
    PLUGIN_MANUFACTURER_CODE Arch
    PLUGIN_CODE SpGt
    FORMATS VST3 AU Standalone
    PRODUCT_NAME "Spectral Gate"
    IS_SYNTH FALSE
)

juce_generate_juce_header(SpectralGate)

target_sources(SpectralGate
    PRIVATE
        Source/PluginProcessor.cpp
        Source/PluginProcessor.h
        Source/PluginEditor.cpp
        Source/PluginEditor.h
)

target_compile_definitions(SpectralGate
    PRIVATE
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_VST3_CAN_REPLACE_VST2=0
)

target_link_libraries(SpectralGate
    PRIVATE
        juce::juce_audio_utils
        juce::juce_dsp
)