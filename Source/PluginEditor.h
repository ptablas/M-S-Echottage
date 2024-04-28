/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class MSUtilityAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public: 
    MSUtilityAudioProcessorEditor (MSUtilityAudioProcessor&,
juce::AudioProcessorValueTreeState&);
    ~MSUtilityAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:

    //Sliders&ComboBoxes                <- Objects to be painted are defined;

    juce::Slider sendDial;
    juce::Slider timeDial;
    juce::Slider LFOSpeedDial;
    juce::Slider LFODepthDial;
    juce::ComboBox waveformDial;
    juce::Slider feedbackDial;

    //Values                            <- Values for treeState

    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> sendValue;
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> timeValue;
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> LFOSpeedValue;
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> LFODepthValue;
    std::unique_ptr <juce::AudioProcessorValueTreeState::ComboBoxAttachment> waveformValue;
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> feedbackValue;

    MSUtilityAudioProcessor& audioProcessor;
    juce::AudioProcessorValueTreeState& treeState;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MSUtilityAudioProcessorEditor)
};
