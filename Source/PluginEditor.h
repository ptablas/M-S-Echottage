/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

#include "CheckBackground.h"
#include "WoodFrame.h"
#include "Oscilloscope.h"

//==============================================================================
/**
*/
class MSUtilityAudioProcessorEditor  : public juce::AudioProcessorEditor,
                                       public juce::Timer
{
public: 
    MSUtilityAudioProcessorEditor (MSUtilityAudioProcessor&,
juce::AudioProcessorValueTreeState&);
    ~MSUtilityAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    void timerCallback() override;

private:
    // Sliders and ComboBoxes
    juce::Slider sendDial;
    juce::Slider timeDial;
    juce::Slider LFOSpeedDial;
    juce::Slider LFODepthDial;
    juce::Slider waveformDial;
    juce::Slider feedbackDial;

    // ValueTreeState Values
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> sendValue;
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> timeValue;
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> LFOSpeedValue;
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> LFODepthValue;
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> waveformValue;
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> feedbackValue;

    // Custom Components
    CheckBackground checkBackground;
    WoodFrame woodFrame;
    Oscilloscope oscilloscope;
    
    // Plugin utilities
    MSUtilityAudioProcessor& audioProcessor;
    juce::AudioProcessorValueTreeState& treeState;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MSUtilityAudioProcessorEditor)
};
