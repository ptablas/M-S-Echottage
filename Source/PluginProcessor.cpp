/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

#include "Osc.h"

//==============================================================================
MSUtilityAudioProcessor::MSUtilityAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
        .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
        .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
    ), treeState(*this, nullptr, juce::Identifier("PARAMETERS"),
        {
           // Definition of parameters IDs, ranges and starting values to be passed to TreeState

           std::make_unique<juce::AudioParameterFloat>("send", "Send", 0.f, 1.f, 0.f), //controls dry/wet of signal
           std::make_unique<juce::AudioParameterFloat>("time", "Time", 0.f, 20000.f, 0.f), // Delay time in samples
           std::make_unique<juce::AudioParameterFloat>("lfospeed", "LFOSpeed", juce::NormalisableRange<float> {0.f, 10.f, 0.0001f, 0.6f}, 0.f), //in Hertz
           std::make_unique<juce::AudioParameterFloat>("lfodepth", "LFODepth", juce::NormalisableRange<float> {0.f, 20000.f / 2.f, 0.0001f, 0.6f}, 0.f), // in miliseconds (since it modulates time) Again skew factor
           std::make_unique<juce::AudioParameterChoice>("waveform", "Waveform", juce::StringArray("Sine", "Triangle", "Sawtooth", "Square", "Random", "Sample & Hold"), 0),
           std::make_unique<juce::AudioParameterFloat>("feedback", "Feedback", 0.f, 0.9f, 0.0001f),
         })
#endif
{
    const juce::StringArray params = { "send", "time", "lfospeed", "lfodepth", "waveform", "feedback"};
    for (int i = 0; i <= 6; i++)
    {
        //adds a listener to each parameter in the array.
        treeState.addParameterListener(params[i], this);
    }
}

MSUtilityAudioProcessor::~MSUtilityAudioProcessor()
{
}

//==============================================================================
const juce::String MSUtilityAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool MSUtilityAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool MSUtilityAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool MSUtilityAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double MSUtilityAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int MSUtilityAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int MSUtilityAudioProcessor::getCurrentProgram()
{
    return 0;
}

void MSUtilityAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String MSUtilityAudioProcessor::getProgramName (int index)
{
    return {};
}

void MSUtilityAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void MSUtilityAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec;                     // Here we create the processSpec type spec variable, 
    spec.maximumBlockSize = samplesPerBlock;         // which holds samples per block of audio, 
    spec.sampleRate = sampleRate;                    // sample rate and number of outputs to be passed to
    spec.numChannels = getTotalNumOutputChannels();  // the prepare function of other DSP modules

    lfoOsc.prepare(spec);

    // Delay Modules Initializiation                    << Delays here and so on...

    delayModule.reset();
    delayModule.prepare(spec);

    //SmoothedValues -> Creates linear interpolation in parameter changes.
    
    float rampTime = 0.02;

        //GUI

    timeTargetTemp.reset(sampleRate, rampTime);
    LFODepthTarget.reset(sampleRate, rampTime);
    LFOSpeedTarget.reset(sampleRate, rampTime);          
}

void MSUtilityAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool MSUtilityAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void MSUtilityAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    double sampeleratero = getSampleRate();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i) // Clears channels from trash data
        buffer.clear(i, 0, buffer.getNumSamples());
    {
        auto* channelDataLeft = buffer.getWritePointer(0);
        auto* channelDataRight = buffer.getWritePointer(1);

        for (int sample = 0; sample < buffer.getNumSamples(); ++sample) // Sample Processing
        {
            // Mono Signal
            float monoIn = channelDataLeft[sample] + channelDataRight[sample];

            // LFO Phase Calculation <- needed for LFOs
            lfoOut = lfoOsc.output(LFOSpeedTarget.getNextValue(), LFODepthTarget.getNextValue(), &monoIn);

            // Time Modulation -> Time Ramped Value added to LFOs'; lambda makes value always positive
            timeTemp = [](double Time) {if (Time >= 0) { return Time; } else { return -Time; }  }(timeTargetTemp.getNextValue() + lfoOut);

            // Delay

            float dry = monoIn;
            float wet = delayModule.popSample(0, timeTemp);                 // Read a delayed sample
            delayModule.pushSample(0, monoIn + (wet * feedbackTemp));       // Write a sample into buffer + feedback
            float monoOut = (dry * (sendTemp - 1)) + (wet * sendTemp);         // Dry + Wet signals

            // Output Handling
            channelDataLeft[sample] = monoOut;
            channelDataRight[sample] = monoOut;
        }
    }
}

//==============================================================================
bool MSUtilityAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* MSUtilityAudioProcessor::createEditor()
{
    return new MSUtilityAudioProcessorEditor (*this, treeState);
}

//==============================================================================
void MSUtilityAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.

    auto state = treeState.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void MSUtilityAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.

    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));

    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName(treeState.state.getType()))
            treeState.replaceState(juce::ValueTree::fromXml(*xmlState));
}

//Function called when parameter is changed
void MSUtilityAudioProcessor::parameterChanged(const juce::String& parameterID, float newValue)
{
    // Here we detect changes made by parameterIDs in the editor for processing in the processBlock.

    if (parameterID == "send")
    {
        sendTemp = newValue;
    }
    else if (parameterID == "time")
    {

        timeTargetTemp.setTargetValue(newValue);

    }
    else if (parameterID == "lfospeed")
    {
        LFOSpeedTarget.setTargetValue(newValue);
    }
    else if (parameterID == "lfodepth")
    {
        LFODepthTarget.setTargetValue(newValue);
    }

    else if (parameterID == "waveform")
    {
        switch ((int)newValue)
        {
        case 0:
            lfoOsc.setWaveform(Osc::Waveform::Sine);
            break;
        case 1:
            lfoOsc.setWaveform(Osc::Waveform::Triangle);
            break;
        case 2:
            lfoOsc.setWaveform(Osc::Waveform::Sawtooth);
            break;
        case 3:
            lfoOsc.setWaveform(Osc::Waveform::Square);
            break;
        case 4:
            lfoOsc.setWaveform(Osc::Waveform::Random);
            break;
        case 5:
            lfoOsc.setWaveform(Osc::Waveform::SH);
            break;
        }
    }
    else if (parameterID == "feedback")
    {
        feedbackTemp = newValue;
    }
}


//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MSUtilityAudioProcessor();
}
