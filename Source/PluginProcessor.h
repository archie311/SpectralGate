#pragma once

#include <JuceHeader.h>
#include <array>

class SpectralGateAudioProcessor : public juce::AudioProcessor
{
public:
    SpectralGateAudioProcessor();
    ~SpectralGateAudioProcessor() override = default;

    juce::AudioProcessorValueTreeState apvts;

    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    bool isBusesLayoutSupported(const BusesLayout& layouts) const override;

    void processBlock(
        juce::AudioBuffer<float>&,
        juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;

    double getTailLengthSeconds() const override;

    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram(int index) override;

    const juce::String getProgramName(int index) override;

    void changeProgramName(
        int index,
        const juce::String& newName) override;

    void getStateInformation(
        juce::MemoryBlock& destData) override;

    void setStateInformation(
        const void* data,
        int sizeInBytes) override;

private:
    static constexpr int fftOrder = 10;
    static constexpr int fftSize = 1 << fftOrder;

    static constexpr int hopSize = fftSize / 2;

    static constexpr int outputBufferSize = fftSize * 2;

    juce::dsp::FFT fft;

    juce::dsp::WindowingFunction<float> window;

    std::array<float, fftSize * 2> fftData {};

    juce::AudioBuffer<float> fifo;
    int fifoIndex = 0;
    int samplesSinceLastHop = 0;

    juce::AudioBuffer<float> outputFifo;

    int outputReadIndex = 0;
    int outputWriteIndex = 0;

    std::array<float, fftSize / 2 + 1> binGains {};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(
        SpectralGateAudioProcessor)
};

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter();
