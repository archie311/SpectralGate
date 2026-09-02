#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

class SpectralGateAudioProcessorEditor
    : public juce::AudioProcessorEditor
{
public:
    explicit SpectralGateAudioProcessorEditor(
        SpectralGateAudioProcessor&);
    
    ~SpectralGateAudioProcessorEditor() override = default;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    SpectralGateAudioProcessor& audioProcessor;

    juce::Slider thresholdSlider;
    juce::Label thresholdLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> thresholdAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(
        SpectralGateAudioProcessorEditor)
};