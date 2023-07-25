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

    //Sliders&ComboBoxes                <- Objects to be painted are defined

        //Width Section
    juce::Slider stereowidthDial;
    juce::ComboBox inputSel;
    juce::ComboBox outputSel;

        //Filter Section

            //Mid
    juce::Slider cutoffmidDial;
    juce::Slider resonancemidDial;
    juce::ComboBox modemidSel;

            //Side
    juce::Slider cutoffsideDial;
    juce::Slider resonancesideDial;
    juce::ComboBox modesideSel;

        //Delay Section

            //Mid
    juce::Slider sendmidDial;
    juce::Slider timemidDial;
    juce::Slider lfospeedmidDial;
    juce::Slider lfodepthmidDial;
    juce::ComboBox waveformmidSel;
    juce::Slider feedbackmidDial;

            //Side
    juce::Slider sendsideDial;
    juce::Slider timesideDial;
    juce::Slider lfospeedsideDial;
    juce::Slider lfodepthsideDial;
    juce::ComboBox waveformsideSel;
    juce::Slider feedbacksideDial;

    //Values                            <- Values for treeState

        //Width Section
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> stereowidthValue;
    std::unique_ptr <juce::AudioProcessorValueTreeState::ComboBoxAttachment> inputChoice;
    std::unique_ptr <juce::AudioProcessorValueTreeState::ComboBoxAttachment> outputChoice;


        //Filter Section

            //Mid
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> cutoffmidValue;
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> resonancemidValue;
    std::unique_ptr <juce::AudioProcessorValueTreeState::ComboBoxAttachment> modemidChoice;

        //Side
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> cutoffsideValue;
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> resonancesideValue;
    std::unique_ptr <juce::AudioProcessorValueTreeState::ComboBoxAttachment> modesideChoice;

        //Delay Section

           //Mid
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> sendmidValue;
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> timemidValue;
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> lfospeedmidValue;
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> lfodepthmidValue;
    std::unique_ptr <juce::AudioProcessorValueTreeState::ComboBoxAttachment> waveformmidChoice;
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> feedbackmidValue;

            //Side
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> sendsideValue;
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> timesideValue;
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> lfospeedsideValue;
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> lfodepthsideValue;
    std::unique_ptr <juce::AudioProcessorValueTreeState::ComboBoxAttachment> waveformsideChoice;
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> feedbacksideValue;

    MSUtilityAudioProcessor& audioProcessor;
    juce::AudioProcessorValueTreeState& treeState;



    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MSUtilityAudioProcessorEditor)
};
