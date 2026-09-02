#include "PluginProcessor.h"
#include "PluginEditor.h"

namespace
{
    juce::AudioProcessorValueTreeState::ParameterLayout
    createParameterLayout()
    {
        std::vector<std::unique_ptr<juce::RangedAudioParameter>>
            parameters;

        parameters.push_back(
            std::make_unique<juce::AudioParameterFloat>(
                "threshold",
                "Threshold",
                -60.0f,
                60.0f,
                -30.0f
            )
        );

        return {
            parameters.begin(),
            parameters.end()
        };
    }
}

SpectralGateAudioProcessor::SpectralGateAudioProcessor()

#ifndef JucePlugin_PreferredChannelConfigurations

    : AudioProcessor(
        BusesProperties()
            .withInput(
                "Input",
                juce::AudioChannelSet::stereo(),
                true)
            .withOutput(
                "Output",
                juce::AudioChannelSet::stereo(),
                true)),

      apvts(
          *this,
          nullptr,
          "Parameters",
          createParameterLayout()),

      fft(fftOrder),

      window(
          fftSize,
          juce::dsp::WindowingFunction<float>::hann,
          false)

#endif

{
}

void SpectralGateAudioProcessor::prepareToPlay(
    double sampleRate,
    int samplesPerBlock)
{
    fifo.setSize(1, fftSize);
    fifo.clear();

    outputFifo.setSize(1, outputBufferSize);
    outputFifo.clear();

    fifoIndex = 0;
    samplesSinceLastHop = 0;

    outputReadIndex = 0;
    outputWriteIndex = 0;

    binGains.fill(1.0f);
}

void SpectralGateAudioProcessor::releaseResources()
{
}

bool SpectralGateAudioProcessor::isBusesLayoutSupported(
    const BusesLayout& layouts) const
{
    if (layouts.getMainOutputChannelSet()
        != juce::AudioChannelSet::stereo())
    {
        return false;
    }

    return layouts.getMainInputChannelSet()
        == layouts.getMainOutputChannelSet();
}

void SpectralGateAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer&)
{
    juce::ScopedNoDenormals noDenormals;
    const int numSamples = buffer.getNumSamples();

    const float attackCoefficient = 0.05f;
    const float releaseCoefficient = 0.005f;
    const float threshold = apvts.getRawParameterValue("threshold")->load();

    for (int i = 0; i < numSamples; ++i)
    {
        fifo.setSample(0, fifoIndex, buffer.getSample(0, i));
        fifoIndex = (fifoIndex + 1) % fftSize;
        ++samplesSinceLastHop;

        if (samplesSinceLastHop == hopSize)
        {
            samplesSinceLastHop = 0;

            int readPtr = fifoIndex; 
            for (int n = 0; n < fftSize; ++n)
            {
                fftData[n] = fifo.getSample(0, readPtr);
                readPtr = (readPtr + 1) % fftSize;
            }

            window.multiplyWithWindowingTable(fftData.data(), fftSize);

            fft.performRealOnlyForwardTransform(fftData.data());

            const int numBins = fftSize / 2 + 1;

            for (int bin = 0; bin < numBins; ++bin)
            {
                float real = 0.0f;
                float imag = 0.0f;

                if (bin == 0) 
                {
                    real = fftData[0];
                    imag = 0.0f;
                }
                else if (bin == numBins - 1)
                {
                    real = fftData[1];
                    imag = 0.0f;
                }
                else 
                {
                    real = fftData[2 * bin];
                }

                const float magnitude = std::sqrt(real * real + imag * imag);
                const float magnitudeDb = juce::Decibels::gainToDecibels(magnitude);

                float targetGain = (magnitudeDb < threshold) ? 0.05f : 1.0f; 

                if (targetGain > binGains[bin])
                    binGains[bin] += attackCoefficient * (targetGain - binGains[bin]);
                else
                    binGains[bin] += releaseCoefficient * (targetGain - binGains[bin]);

                if (bin == 0) 
                {
                    fftData[0] *= binGains[bin];
                }
                else if (bin == numBins - 1)
                {
                    fftData[1] *= binGains[bin];
                }
                else 
                {
                    fftData[2 * bin] *= binGains[bin];
                    fftData[2 * bin + 1] *= binGains[bin];
                }
            }

            fft.performRealOnlyInverseTransform(fftData.data());

            window.multiplyWithWindowingTable(fftData.data(), fftSize);

            for (int n = 0; n < fftSize; ++n)
            {
                const int index = (outputWriteIndex + n) % outputBufferSize;
                outputFifo.addSample(0, index, fftData[n]);
            }

            outputWriteIndex = (outputWriteIndex + hopSize) % outputBufferSize;
        }

        float outputSample = outputFifo.getSample(0, outputReadIndex);
        outputFifo.setSample(0, outputReadIndex, 0.0f);
        outputReadIndex = (outputReadIndex + 1) % outputBufferSize;

        for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
        {
            buffer.setSample(channel, i, outputSample);
        }
    }
}

juce::AudioProcessorEditor*
SpectralGateAudioProcessor::createEditor()
{
    return new SpectralGateAudioProcessorEditor(*this);
}

bool SpectralGateAudioProcessor::hasEditor() const
{
    return true;
}

const juce::String
SpectralGateAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool SpectralGateAudioProcessor::acceptsMidi() const
{
    return false;
}

bool SpectralGateAudioProcessor::producesMidi() const
{
    return false;
}

bool SpectralGateAudioProcessor::isMidiEffect() const
{
    return false;
}

double SpectralGateAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int SpectralGateAudioProcessor::getNumPrograms()
{
    return 1;
}

int SpectralGateAudioProcessor::getCurrentProgram()
{
    return 0;
}

void SpectralGateAudioProcessor::setCurrentProgram(
    int index)
{
}

const juce::String
SpectralGateAudioProcessor::getProgramName(
    int index)
{
    return {};
}

void SpectralGateAudioProcessor::changeProgramName(
    int index,
    const juce::String& newName)
{
}

void SpectralGateAudioProcessor::getStateInformation(
    juce::MemoryBlock& destData)
{
}

void SpectralGateAudioProcessor::setStateInformation(
    const void* data,
    int sizeInBytes)
{
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SpectralGateAudioProcessor();
}