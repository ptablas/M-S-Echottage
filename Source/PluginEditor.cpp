/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MSUtilityAudioProcessorEditor::MSUtilityAudioProcessorEditor 
(MSUtilityAudioProcessor& p, juce::AudioProcessorValueTreeState& vts)
    : AudioProcessorEditor (&p), audioProcessor (p), treeState(vts)
{
    startTimerHz(60);

    // Custom Components
    addAndMakeVisible (checkBackground);
    addAndMakeVisible (woodFrame);
    addAndMakeVisible (oscilloscope);

    /* Here all user - interactive elements, their ranges and value steps are 
    defined and attached to a value from the treeState */
    
    // FXs Send
    sendValue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
        (treeState, "send", sendDial);
    sendDial.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    sendDial.setRange(0.f, 1.0f, 0.0001f);
    sendDial.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    addAndMakeVisible(&sendDial);

    // Delay Time
    timeValue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
        (treeState, "time", timeDial);
    timeDial.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    timeDial.setRange(0.f, 20000.f, 0.0001f);
    timeDial.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    addAndMakeVisible(&timeDial);

    // LFO Speed
    LFOSpeedValue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
        (treeState, "lfospeed", LFOSpeedDial);
    LFOSpeedDial.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    LFOSpeedDial.setRange(0.f, 10.f, 0.0001f);
    LFOSpeedDial.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    addAndMakeVisible(&LFOSpeedDial);

    // LFO Depth
    LFODepthValue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
        (treeState, "lfodepth", LFODepthDial);
    LFODepthDial.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    LFODepthDial.setRange(0.f, 20000.f / 2.f, 0.0001f);
    LFODepthDial.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    addAndMakeVisible(&LFODepthDial);

    // Waveform
    waveformValue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
        (treeState, "waveform", waveformDial);
    waveformDial.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    waveformDial.setRange(0, 5, 0);
    waveformDial.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    addAndMakeVisible(&waveformDial);

    // Delay Feedback
    feedbackValue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
        (treeState, "feedback", feedbackDial);
    feedbackDial.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    feedbackDial.setRange(0.f, 0.9f, 0.0001f);
    feedbackDial.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    addAndMakeVisible(&feedbackDial);

    setSize(600, 500);
}

MSUtilityAudioProcessorEditor::~MSUtilityAudioProcessorEditor()
{
}

//==============================================================================
void MSUtilityAudioProcessorEditor::paint (juce::Graphics& g)
{
    // Main Background
    checkBackground.setBounds(getLocalBounds());
    
    static constexpr int sphereX = 100;
    static constexpr int sphereY = 0;
    static constexpr int sphereSide = 500;

    const juce::Rectangle<int> sphereBounds(sphereX, sphereY, sphereSide, sphereSide);

    woodFrame.setBounds(sphereBounds);
    oscilloscope.setBounds(sphereBounds.reduced(20));

    //static constexpr int letterWidth = 250;
    //static constexpr int letterHeight = 10;

    // Lettering

    //g.setColour(juce::Colours::beige);
    //g.drawFittedText("LFO", boxXmid + 10, 445, letterWidth, letterHeight, juce::Justification::centredLeft, 1, 0.0f);

    //g.setColour(juce::Colours::rosybrown);
    //g.setFont(juce::Font("Times New Roman", 20.f, juce::Font::italic));

    //g.drawFittedText("fx", 72.5, 265, letterWidth, letterHeight, juce::Justification::centredLeft, 1, 0.0f);
    //g.drawFittedText("t", 77.5, 337.5, letterWidth, letterHeight, juce::Justification::centredLeft, 1, 0.0f);
    //g.drawFittedText("f", 77.5, 170, letterWidth, letterHeight, juce::Justification::centredLeft, 1, 0.0f);
    //g.drawFittedText("fb", 72.5, 545, letterWidth, letterHeight, juce::Justification::centredLeft, 1, 0.0f);

    //g.setFont(juce::Font("Times New Roman", 25.f, juce::Font::italic));
    //g.setColour(juce::Colours::beige);

    //g.drawFittedText("hz", 70, 407.5, letterWidth, letterHeight, juce::Justification::centredLeft, 1, 0.0f);
    //g.drawFittedText("A", 72.5, 477.5, letterWidth, letterHeight, juce::Justification::centredLeft, 1, 0.0f);
}

void MSUtilityAudioProcessorEditor::resized()
{
    static constexpr int dialSide = 90;

    /* The dials are positioned imagining triangles at the top left and bottom
    left corners, where triSide is the side length of an equilateral triangle */

    // Dial Positioning
    static constexpr int triSide = 80;
    static constexpr int halfSide = triSide / 2;

    static constexpr int xRef = 0;
    static constexpr int topYRef = 0;
    static const int bottomYRef = getHeight() - (topYRef + dialSide);

    // Delay Triangle
    const juce::Rectangle<int> delayLeftDial  (xRef,            topYRef,              dialSide, dialSide);
    const juce::Rectangle<int> delayRightDial (xRef + triSide,  topYRef,              dialSide, dialSide);
    const juce::Rectangle<int> delayBottomDial(xRef + halfSide, topYRef + triSide,    dialSide, dialSide);

    // LFO Triangle
    const juce::Rectangle<int> LFOLeftDial    (xRef,            bottomYRef,           dialSide, dialSide);
    const juce::Rectangle<int> LFORightDial   (xRef + triSide,  bottomYRef,           dialSide, dialSide);
    const juce::Rectangle<int> LFOTopDial     (xRef + halfSide, bottomYRef - triSide, dialSide, dialSide);

    // Component Bounds
    sendDial.setBounds(delayLeftDial);
    timeDial.setBounds(delayRightDial);
    feedbackDial.setBounds(delayBottomDial);

    LFODepthDial.setBounds(LFOLeftDial);
    waveformDial.setBounds(LFORightDial);
    LFOSpeedDial.setBounds(LFOTopDial);

    // Component Properties
    sendDial.setPopupDisplayEnabled(true, true, NULL, 2000);
    timeDial.setPopupDisplayEnabled(true, true, NULL, 2000);
    feedbackDial.setPopupDisplayEnabled(true, true, NULL, 2000);
    LFODepthDial.setPopupDisplayEnabled(true, true, NULL, 2000);
    LFOSpeedDial.setPopupDisplayEnabled(true, true, NULL, 2000);
    waveformDial.setPopupDisplayEnabled(true, true, NULL, 2000);
}

void MSUtilityAudioProcessorEditor::timerCallback()
{
    oscilloscope.popSample(audioProcessor.getTimeModulation());
}
