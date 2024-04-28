/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Osc.h"

//==============================================================================
/**
*/
class MSUtilityAudioProcessor  : public juce::AudioProcessor,
                                 public juce::AudioProcessorValueTreeState::Listener
{
public:
    //==============================================================================
    MSUtilityAudioProcessor();
    ~MSUtilityAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    void parameterChanged(const juce::String& parameterID, float newValue) override;

private:
    juce::AudioProcessorValueTreeState treeState;

    // Initialization of Variables for parameterChanged (ultimately for processing block)

    // Initialize Delay         Lagrange3rd is a high-quality interpolation <-> 3000 is longest num. of samples of delay tap

    juce::dsp::DelayLine<double, juce::dsp::DelayLineInterpolationTypes::Lagrange3rd> delayModule{30000};

    double sendTemp = 0.f;
    juce::SmoothedValue<double> timeTargetTemp = 0.f;
    double timeTemp = 0.f;
    double feedbackTemp = 0.f;

    //LFO Variables

    Osc lfoOsc;
    juce::SmoothedValue<double> LFOSpeedTarget = 0;
    juce::SmoothedValue<double> LFODepthTarget = 0;
    double lfoOut = 0;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MSUtilityAudioProcessor)
};
