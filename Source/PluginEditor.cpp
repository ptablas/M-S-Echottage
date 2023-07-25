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
    setSize (300, 600);         // a pluging twice as long than wide

   // Here all user-interactive elements, their ranges and value steps are defined and attached to a value from the treeState


   //WidthSection     

       // stereowidthDial&Value
    stereowidthValue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
        (treeState, "stereowidth", stereowidthDial);
    stereowidthDial.setSliderStyle(juce::Slider::LinearHorizontal);
    stereowidthDial.setRange(0.f, 2.0f, 0.0001f);
    stereowidthDial.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    addAndMakeVisible(&stereowidthDial);
        
        //inputChoice&Sel
    inputSel.addItem("Stereo", 1);
    inputSel.addItem("Mid/Side", 2);
    inputChoice = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(treeState, "input", inputSel);
    addAndMakeVisible(&inputSel);

        //outputChoice&Sel
    outputSel.addItem("Stereo", 1);
    outputSel.addItem("Mid/Side", 2);
    outputChoice = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(treeState, "output", outputSel);
    addAndMakeVisible(&outputSel);

    //FilterSection

        //Mid

            //CutoffDial&Value
    cutoffmidValue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
        (treeState, "cutoffmid", cutoffmidDial);
    cutoffmidDial.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    cutoffmidDial.setRange(20.0f, 20000.0f, 0.0001f);
    cutoffmidDial.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    addAndMakeVisible(&cutoffmidDial);

            //ResonanceDial&Value
    resonancemidValue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
        (treeState, "resonancemid", resonancemidDial);
    resonancemidDial.setSliderStyle(juce::Slider::LinearVertical);
    resonancemidDial.setRange(0.0001, 0.7f, 0.0001f);
    resonancemidDial.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    addAndMakeVisible(&resonancemidDial);

            //Filter Type Choice&Sel
    modemidSel.addItem("LPF", 1);
    modemidSel.addItem("BPF", 2);
    modemidSel.addItem("HPF", 3);
    modemidChoice = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(treeState, "modemid", modemidSel);
    addAndMakeVisible(&modemidSel);

        //Side

            //CutoffDial&Value
    cutoffsideValue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
        (treeState, "cutoffside", cutoffsideDial);
    cutoffsideDial.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    cutoffsideDial.setRange(20.0f, 20000.0f, 0.0001f);
    cutoffsideDial.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    addAndMakeVisible(&cutoffsideDial);

             //ResonanceDial&Value
    resonancesideValue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
        (treeState, "resonanceside", resonancesideDial);
    resonancesideDial.setSliderStyle(juce::Slider::LinearVertical);
    resonancesideDial.setRange(0.0001, 0.7f, 0.0001f);
    resonancesideDial.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    addAndMakeVisible(&resonancesideDial);

            //Filter Type Choice&Sel
    modesideSel.addItem("LPF", 1);
    modesideSel.addItem("BPF", 2);
    modesideSel.addItem("HPF", 3);
    modesideChoice = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(treeState, "modeside", modesideSel);
    addAndMakeVisible(&modesideSel);

    // Delay Section

        //Mid

            // sendDial&Value
    sendmidValue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
        (treeState, "sendmid", sendmidDial);
    sendmidDial.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    sendmidDial.setRange(0.f, 1.0f, 0.0001f);
    sendmidDial.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    addAndMakeVisible(&sendmidDial);

            // timeDial&Value
    timemidValue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
        (treeState, "timemid", timemidDial);
    timemidDial.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    timemidDial.setRange(0.f, 20000.f, 0.0001f);
    timemidDial.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    addAndMakeVisible(&timemidDial);

            // lfospeedDial&Value
    lfospeedmidValue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
        (treeState, "lfospeedmid", lfospeedmidDial);
    lfospeedmidDial.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    lfospeedmidDial.setRange(0.f, 10.f, 0.0001f);
    lfospeedmidDial.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    addAndMakeVisible(&lfospeedmidDial);

            // lfodepthDial&Value
    lfodepthmidValue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
        (treeState, "lfodepthmid", lfodepthmidDial);
    lfodepthmidDial.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    lfodepthmidDial.setRange(0.f, 20000.f / 2.f, 0.0001f);
    lfodepthmidDial.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    addAndMakeVisible(&lfodepthmidDial);

            // Waveform Type Choice&Sel
    waveformmidSel.addItem("Sine", 1);
    waveformmidSel.addItem("Triangle", 2);
    waveformmidSel.addItem("Sawtooth", 3);
    waveformmidSel.addItem("Square", 4);
    waveformmidSel.addItem("Random", 5);
    waveformmidSel.addItem("Sample & Hold", 6);
    waveformmidChoice = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(treeState, "waveformmid", waveformmidSel);
    addAndMakeVisible(&waveformmidSel);

            // feedbackDial&Value
    feedbackmidValue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
        (treeState, "feedbackmid", feedbackmidDial);
    feedbackmidDial.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    feedbackmidDial.setRange(0.f, 0.9f, 0.0001f);
    feedbackmidDial.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    addAndMakeVisible(&feedbackmidDial);

        //Side
 
            // sendDial&Value
    sendsideValue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
        (treeState, "sendside", sendsideDial);
    sendsideDial.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    sendsideDial.setRange(0.f, 1.0f, 0.0001f);
    sendsideDial.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    addAndMakeVisible(&sendsideDial);

    // timeDial&Value
    timesideValue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
        (treeState, "timeside", timesideDial);
    timesideDial.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    timesideDial.setRange(0.f, 20000.f, 0.0001f);
    timesideDial.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    addAndMakeVisible(&timesideDial);

    // lfospeedDial&Value
    lfospeedsideValue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
        (treeState, "lfospeedside", lfospeedsideDial);
    lfospeedsideDial.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    lfospeedsideDial.setRange(0.f, 10.f, 0.0001f);
    lfospeedsideDial.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    addAndMakeVisible(&lfospeedsideDial);

    // lfodepthDial&Value
    lfodepthsideValue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
        (treeState, "lfodepthside", lfodepthsideDial);
    lfodepthsideDial.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    lfodepthsideDial.setRange(0.f, 20000.f / 2.f, 0.0001f);
    lfodepthsideDial.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    addAndMakeVisible(&lfodepthsideDial);

    // Waveform Type Choice&Sel
    waveformsideSel.addItem("Sine", 1);
    waveformsideSel.addItem("Triangle", 2);
    waveformsideSel.addItem("Sawtooth", 3);
    waveformsideSel.addItem("Square", 4);
    waveformsideSel.addItem("Random", 5);
    waveformsideSel.addItem("Sample & Hold", 6);
    waveformsideChoice = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(treeState, "waveformside", waveformsideSel);
    addAndMakeVisible(&waveformsideSel);

    // feedbackDial&Value
    feedbacksideValue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
        (treeState, "feedbackside", feedbacksideDial);
    feedbacksideDial.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    feedbacksideDial.setRange(0.f, 0.9f, 0.0001f);
    feedbacksideDial.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    addAndMakeVisible(&feedbacksideDial);

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


    //WidthSection

    stereowidthDial.setBounds(20, 50, 260, 100);
    stereowidthDial.setPopupDisplayEnabled(true, true, NULL, 2000);

    inputSel.setBounds(30, 60, 20, 20);
    outputSel.setBounds(70, 60, 20, 20);

    //FilterSection

    float dialXmid = (getWidth() / 10) + 5; // dialMid is the X position for the left side dials
    float dialXside = 172.5f;           // while dialSide is the X position for the right-hand side.
    float dialWidth = 90;
    float dialHeight = 85;

        //Mid

    
    cutoffmidDial.setBounds(dialXmid, 136, dialWidth, dialHeight);
    cutoffmidDial.setPopupDisplayEnabled(true, true, NULL, 2000);

    resonancemidDial.setBounds(112.5, 140, 20, 75);
    resonancemidDial.setPopupDisplayEnabled(true, true, NULL, 2000);

    modemidSel.setBounds(25, 140, 20, 20);

        //Side
    cutoffsideDial.setBounds(dialXside, 136, dialWidth, dialHeight);
    cutoffsideDial.setPopupDisplayEnabled(true, true, NULL, 2000);

    resonancesideDial.setBounds(252.5, 140, 20, 75);
    resonancesideDial.setPopupDisplayEnabled(true, true, NULL, 2000);

    modesideSel.setBounds(167.5, 140, 20, 20);

    //DelaySection

        //Mid
    sendmidDial.setBounds(dialXmid, 230, dialWidth, dialHeight);
    sendmidDial.setPopupDisplayEnabled(true, true, NULL, 2000);

    timemidDial.setBounds(dialXmid, 300, dialWidth, dialHeight);
    timemidDial.setPopupDisplayEnabled(true, true, NULL, 2000);

    lfospeedmidDial.setBounds(dialXmid, 370, dialWidth, dialHeight);
    lfospeedmidDial.setPopupDisplayEnabled(true, true, NULL, 2000);

    lfodepthmidDial.setBounds(dialXmid, 440, dialWidth, dialHeight);
    lfodepthmidDial.setPopupDisplayEnabled(true, true, NULL, 2000);

    waveformmidSel.setBounds(110, 380, 20, 20);

    feedbackmidDial.setBounds(dialXmid, 510, dialWidth, dialHeight);
    feedbackmidDial.setPopupDisplayEnabled(true, true, NULL, 2000);


        //Side
    sendsideDial.setBounds(dialXside, 230, dialWidth, dialHeight);
    sendsideDial.setPopupDisplayEnabled(true, true, NULL, 2000);

    timesideDial.setBounds(dialXside, 300, dialWidth, dialHeight);
    timesideDial.setPopupDisplayEnabled(true, true, NULL, 2000);

    lfospeedsideDial.setBounds(dialXside, 370, dialWidth, dialHeight);
    lfospeedsideDial.setPopupDisplayEnabled(true, true, NULL, 2000);

    lfodepthsideDial.setBounds(dialXside, 440, dialWidth, dialHeight);
    lfodepthsideDial.setPopupDisplayEnabled(true, true, NULL, 2000);

    waveformsideSel.setBounds(252.5, 380, 20, 20);

    feedbacksideDial.setBounds(dialXside, 510, dialWidth, dialHeight);
    feedbacksideDial.setPopupDisplayEnabled(true, true, NULL, 2000);
}
