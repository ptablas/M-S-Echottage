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

    /* Here all user - interactive elements, their ranges and value steps are 
    defined and attached to a value from the treeState */

    // sendDial&Value
    sendValue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
        (treeState, "send", sendDial);
    sendDial.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    sendDial.setRange(0.f, 1.0f, 0.0001f);
    sendDial.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    addAndMakeVisible(&sendDial);

            // timeDial&Value
    timeValue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
        (treeState, "time", timeDial);
    timeDial.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    timeDial.setRange(0.f, 20000.f, 0.0001f);
    timeDial.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    addAndMakeVisible(&timeDial);

            // lfospeedDial&Value
    LFOSpeedValue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
        (treeState, "lfospeed", LFOSpeedDial);
    LFOSpeedDial.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    LFOSpeedDial.setRange(0.f, 10.f, 0.0001f);
    LFOSpeedDial.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    addAndMakeVisible(&LFOSpeedDial);

            // lfodepthDial&Value
    LFODepthValue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
        (treeState, "lfodepth", LFODepthDial);
    LFODepthDial.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    LFODepthDial.setRange(0.f, 20000.f / 2.f, 0.0001f);
    LFODepthDial.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    addAndMakeVisible(&LFODepthDial);

            // Waveform Type Choice&Sel
    waveformDial.addItem("Sine", 1);
    waveformDial.addItem("Triangle", 2);
    waveformDial.addItem("Sawtooth", 3);
    waveformDial.addItem("Square", 4);
    waveformDial.addItem("Random", 5);
    waveformDial.addItem("Sample & Hold", 6);
    waveformValue = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(treeState, "waveform", waveformDial);
    addAndMakeVisible(&waveformDial);

            // feedbackDial&Value
    feedbackValue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
        (treeState, "feedback", feedbackDial);
    feedbackDial.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    feedbackDial.setRange(0.f, 0.9f, 0.0001f);
    feedbackDial.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    addAndMakeVisible(&feedbackDial);

    setSize(300, 600);         // a pluging twice as long than wide
}

MSUtilityAudioProcessorEditor::~MSUtilityAudioProcessorEditor()
{
}

//==============================================================================
void MSUtilityAudioProcessorEditor::paint (juce::Graphics& g)
{

    // Main Background
    g.fillAll(juce::Colours::burlywood);
    
    // Boxes

    float boxXmid = 20;
    float boxXside = 162.5;
       
    float boxsmallhorWidth = 120;
    float boxsmallhorHeight = 90;
    float boxcornerSize = 10;
    float linencornersize = 5;
    float linenlinewidth = 2;

    int letterWidth = 250;
    int letterHeight = 10;


    // Inside of Boxes

    g.setColour(juce::Colours::antiquewhite);
    g.fillRoundedRectangle(boxXmid, 55, 262, 60, 10);                                           // Big Horizontal
    g.fillRoundedRectangle(boxXmid, 130, boxsmallhorWidth, boxsmallhorHeight, boxcornerSize);   // Small Horizontal Left
    g.fillRoundedRectangle(boxXside, 130, boxsmallhorWidth, boxsmallhorHeight, boxcornerSize);  // Small Horizontal Right
    g.fillRoundedRectangle(boxXmid, 230, boxsmallhorWidth, 360, boxcornerSize);                 // Big Vertical Left
    g.fillRoundedRectangle(boxXside, 230, boxsmallhorWidth, 360, boxcornerSize);                // Big Vertical Right

    // Linen of Boxes

    g.setColour(juce::Colours::rosybrown);
    g.drawRoundedRectangle(boxXmid, 55, 262, 60, linencornersize, linenlinewidth);                  // Big Horizontal
    g.drawRoundedRectangle(boxXmid, 130, boxsmallhorWidth, boxsmallhorHeight, linencornersize, linenlinewidth); // Small Horizontal Left
    g.drawRoundedRectangle(boxXside, 130, boxsmallhorWidth, boxsmallhorHeight, linencornersize, linenlinewidth);// Small Horizontal Right
    g.drawRoundedRectangle(boxXmid, 230, boxsmallhorWidth, 360, linencornersize, linenlinewidth);   // Big Vertical Left
    g.drawRoundedRectangle(boxXside, 230, boxsmallhorWidth, 360, linencornersize, linenlinewidth);  // Big Vertical Right

   // Checkerboards Boxes

        // def. of rectangle class types (needed for checkerboards)        

    juce::Rectangle<float> titleBoard(20, 10, 260, 35);                                 // Title Box
    juce::Rectangle<float> LFOmidBoard(boxXmid + 10, 375, boxsmallhorWidth - 20, 140);  // LFO mid Box
    juce::Rectangle<float> LFOsideBoard(boxXside + 10, 375, boxsmallhorWidth - 20, 140);// LFO side Box

    g.setColour(juce::Colours::beige);
    g.setFont(juce::Font("Times New Roman", 30.f, juce::Font::italic));
    g.fillCheckerBoard(titleBoard, 10, 10, juce::Colours::rosybrown, juce::Colours::sandybrown);
    g.drawRoundedRectangle(boxXmid, 10, 262, 37, linencornersize, linenlinewidth);
    g.drawFittedText("M/S echo-ttage", 25, 22.5, letterWidth, 10, juce::Justification::centred, 1, 0.0f);
    

    g.setColour(juce::Colours::saddlebrown);
    g.fillCheckerBoard(LFOsideBoard, 10, 10, juce::Colours::rosybrown, juce::Colours::sandybrown);
    g.fillCheckerBoard(LFOmidBoard, 10, 10, juce::Colours::rosybrown, juce::Colours::sandybrown);
    g.drawRoundedRectangle(boxXmid + 9, 374, boxsmallhorWidth - 18, 142, linencornersize, linenlinewidth);
    g.drawRoundedRectangle(boxXside + 9, 374, boxsmallhorWidth - 18, 142, linencornersize, linenlinewidth);

    // Lettering

    g.setColour(juce::Colours::beige);
    g.drawFittedText("LFO", 75, 445, letterWidth, letterHeight, juce::Justification::centred, 1, 0.0f);
    g.drawFittedText("LFO", boxXmid + 10, 445, letterWidth, letterHeight, juce::Justification::centredLeft, 1, 0.0f);

    g.setColour(juce::Colours::rosybrown);

    g.drawFittedText("i", 55, 62.5, letterWidth, letterHeight, juce::Justification::centredLeft, 1, 0.0f);
    g.drawFittedText("o", 95, 62.5, letterWidth, letterHeight, juce::Justification::centredLeft, 1, 0.0f);
    g.drawFittedText("Stereo Width", 80, 82.5, letterWidth, letterHeight, juce::Justification::centred, 1, 0.0f);
    g.drawFittedText("Filter", 75, 200, letterWidth, letterHeight, juce::Justification::verticallyCentred, 1, 0.0f);
    g.drawFittedText("Filter", 217.5, 200, letterWidth, letterHeight, juce::Justification::verticallyCentred, 1, 0.0f);
    g.drawFittedText("Delay", 75, 300, letterWidth, letterHeight, juce::Justification::centredLeft, 1, 0.0f);
    g.drawFittedText("Delay", 32.5, 300, letterWidth, letterHeight, juce::Justification::centredRight, 1, 0.0f);

    g.setFont(juce::Font("Times New Roman", 20.f, juce::Font::italic));

    g.drawFittedText("fx", 72.5, 265, letterWidth, letterHeight, juce::Justification::centredLeft, 1, 0.0f);
    g.drawFittedText("fx", -27.5, 265, letterWidth, letterHeight, juce::Justification::centredRight, 1, 0.0f);

    g.drawFittedText("t", 77.5, 337.5, letterWidth, letterHeight, juce::Justification::centredLeft, 1, 0.0f);
    g.drawFittedText("t", -30, 337.5, letterWidth, letterHeight, juce::Justification::centredRight, 1, 0.0f);

    g.drawFittedText("f", 77.5, 170, letterWidth, letterHeight, juce::Justification::centredLeft, 1, 0.0f);
    g.drawFittedText("f", -30, 170, letterWidth, letterHeight, juce::Justification::centredRight, 1, 0.0f);

    g.drawFittedText("fb", 72.5, 545, letterWidth, letterHeight, juce::Justification::centredLeft, 1, 0.0f);
    g.drawFittedText("fb", -27.5, 545, letterWidth, letterHeight, juce::Justification::centredRight, 1, 0.0f);

    g.setFont(juce::Font("Times New Roman", 25.f, juce::Font::italic));

    g.setColour(juce::Colours::beige);

    g.drawFittedText("hz", 70, 407.5, letterWidth, letterHeight, juce::Justification::centredLeft, 1, 0.0f);
    g.drawFittedText("hz", -22.5, 407.5, letterWidth, letterHeight, juce::Justification::centredRight, 1, 0.0f);

    g.drawFittedText("A", 72.5, 477.5, letterWidth, letterHeight, juce::Justification::centredLeft, 1, 0.0f);
    g.drawFittedText("A", -27.5, 477.5, letterWidth, letterHeight, juce::Justification::centredRight, 1, 0.0f);


    // Section Divisions Line

    float divlinX = 15.f;
    float divlinY = 50.f;
    float divlinWidth = 270.f;
    float divlinHeight = 2.f;
    float divlinCorn = 1.f;

    g.setColour(juce::Colours::rosybrown);


        // Big Horizontal
    g.fillRoundedRectangle(divlinX, 50.f, divlinWidth, divlinHeight, divlinCorn); // Above
    g.fillRoundedRectangle(divlinX, 125.f, divlinWidth, divlinHeight, divlinCorn);// Below

        // Small Horizontal
    g.fillRoundedRectangle(divlinX, 225.f, (divlinWidth / 2) - 10, divlinHeight, divlinCorn);  //Left Side
    g.fillRoundedRectangle(boxXside, 225.f, (divlinWidth / 2) - 10, divlinHeight, divlinCorn);    //Right Side



        // Vertical
    //                    (x               y        width        height               cornerSize)
    g.fillRoundedRectangle(getWidth() / 2, 135.f, divlinHeight , getHeight() - 145.f, divlinCorn);



    //MID SIDE COLUMN LABELS

    g.setColour(juce::Colours::black);
    g.setFont(juce::Font("Times New Roman", 30.f, juce::Font::bold));
    g.drawFittedText("MID", 50, 120, letterWidth, letterHeight, juce::Justification::centredLeft, 1, 0.0f);
    g.drawFittedText("SIDE", 0, 120, letterWidth, letterHeight, juce::Justification::centredRight, 1, 0.0f);
    
    //UNDERLINING
    g.fillRoundedRectangle(divlinX + 20, 135.f, (divlinWidth / 2) - 50, divlinHeight, divlinCorn);  //Left Side
    g.fillRoundedRectangle(boxXside + 17.5, 135.f, (divlinWidth / 2) - 50, divlinHeight, divlinCorn);    //Right Side

}

void MSUtilityAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..

    float dialXmid = (getWidth() / 10) + 5; // dialMid is the X position for the left side dials
    float dialXside = 172.5f;           // while dialSide is the X position for the right-hand side.
    float dialWidth = 90;
    float dialHeight = 85;

    sendDial.setBounds(dialXmid, 230, dialWidth, dialHeight);
    sendDial.setPopupDisplayEnabled(true, true, NULL, 2000);

    timeDial.setBounds(dialXmid, 300, dialWidth, dialHeight);
    timeDial.setPopupDisplayEnabled(true, true, NULL, 2000);

    LFOSpeedDial.setBounds(dialXmid, 370, dialWidth, dialHeight);
    LFOSpeedDial.setPopupDisplayEnabled(true, true, NULL, 2000);

    LFODepthDial.setBounds(dialXmid, 440, dialWidth, dialHeight);
    LFODepthDial.setPopupDisplayEnabled(true, true, NULL, 2000);

    waveformDial.setBounds(110, 380, 20, 20);

    feedbackDial.setBounds(dialXmid, 510, dialWidth, dialHeight);
    feedbackDial.setPopupDisplayEnabled(true, true, NULL, 2000);
}
