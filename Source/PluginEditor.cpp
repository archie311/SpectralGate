#include "PluginEditor.h"

SpectralGateAudioProcessorEditor::SpectralGateAudioProcessorEditor(
    SpectralGateAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p)
{
    setSize(500, 300);
    addAndMakeVisible(thresholdSlider);
    
    thresholdSlider.setRange(-60.0, 0.0, 0.1);
    thresholdSlider.setValue(-30.0);
    thresholdSlider.setTextValueSuffix(" dB");

    thresholdLabel.setText("THRESHOLD", juce::dontSendNotification);
    thresholdLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(thresholdLabel);

    thresholdAttachment =
        std::make_unique<
            juce::AudioProcessorValueTreeState::SliderAttachment
        >(
            audioProcessor.apvts,
            "threshold",
            thresholdSlider
        );
}

void SpectralGateAudioProcessorEditor::paint(
    juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);

    g.setColour(juce::Colours::white);
    g.setFont(24.0f);

    g.drawFittedText(
        "Spectral Gate",
        getLocalBounds(),
        juce::Justification::centred,
        1);
}

void SpectralGateAudioProcessorEditor::resized()
{
    thresholdSlider.setBounds(50, 50, 300, 50);
    thresholdLabel.setBounds(50, 20, 200, 30);
}