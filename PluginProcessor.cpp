/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


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

            //Definition of parameters IDs, ranges and starting values to be passed to TreeState

            //Width Section
            std::make_unique<juce::AudioParameterFloat>("stereowidth", "StereoWidth", 0.f, 2.f, 1.f),
            std::make_unique<juce::AudioParameterChoice>("input", "Input", juce::StringArray("Stereo", "Mid/Side"), 0),
            std::make_unique<juce::AudioParameterChoice>("output", "Output", juce::StringArray("Stereo", "Mid/Side"), 0),

            //Filter Section

                //Mid
            std::make_unique<juce::AudioParameterFloat>("cutoffmid", "cutoffMid", juce::NormalisableRange<float> {20.f, 20000.0f, 0.0001f, 0.6f}, 200.f),  // <-creates skew factor  
            std::make_unique<juce::AudioParameterFloat>("resonancemid", "ResonanceMid", 0.1, 0.7f, 0.0001),                                                //   (more of the dial
            std::make_unique<juce::AudioParameterChoice>("modemid", "Filter Type Mid", juce::StringArray("LPF", "BPF", "HPF"), 0),                         //   affects lower side)

                //Side
            std::make_unique<juce::AudioParameterFloat>("cutoffside", "cutoffSide", juce::NormalisableRange<float> {20.f, 20000.0f, 0.0001f, 0.6f}, 200.f), // skew factor again
            std::make_unique<juce::AudioParameterFloat>("resonanceside", "ResonanceSide", 0.1, 0.7, 0.0001f),
            std::make_unique<juce::AudioParameterChoice>("modeside", "Filter Type Side", juce::StringArray("LPF", "BPF", "HPF"), 0),

           //Delay  Section
                            
                //Mid
           std::make_unique<juce::AudioParameterFloat>("sendmid", "SendMid", 0.f, 1.f, 0.f), //controls dry/wet of signal
           std::make_unique<juce::AudioParameterFloat>("timemid", "TimeMid", 0.f, 2000.f, 0.f), // Delay time in samples
           std::make_unique<juce::AudioParameterFloat>("lfospeedmid", "LFOSpeedMid", juce::NormalisableRange<float> {0.f, 10.f, 0.0001f, 0.6f}, 0.f), //in Hertz
           std::make_unique<juce::AudioParameterFloat>("lfodepthmid", "LFODepthMid", juce::NormalisableRange<float> {0.f, 2000.f / 2.f, 0.0001f, 0.6f}, 0.f), // in samples (since it modulates time) Again skew factor
           std::make_unique<juce::AudioParameterFloat>("feedbackmid", "FeedbackMid", 0.f, 0.8f, 0.0001f),

                //Side
           std::make_unique<juce::AudioParameterFloat>("sendside", "SendSide", 0.f, 1.f, 0.f),
           std::make_unique<juce::AudioParameterFloat>("timeside", "TimeSide", 0.f, 2000.f, 0.f), // In samples
           std::make_unique<juce::AudioParameterFloat>("lfospeedside", "LFOSpeedSide", juce::NormalisableRange<float> {0.f, 10.f, 0.0001f, 0.6f}, 0.f),
           std::make_unique<juce::AudioParameterFloat>("lfodepthside", "LFODepthSide", juce::NormalisableRange<float> {0.f, 2000.f / 2.f, 0.0001f, 0.6f}, 0.f),
           std::make_unique<juce::AudioParameterFloat>("feedbackside", "FeedbackSide", 0.f, 0.8f, 0.0001f),
                           })
#endif
{
    const juce::StringArray params = { "stereowidth", "input", "output", //Width Section
                                       "cutoffmid", "resonancemid", "modemid", //Filter Section -> Mid
                                       "cutoffside", "resonanceside", "modeside", //Filter Section -> Side
                                       "sendmid", "timemid", "lfospeedmid", "lfodepthmid", "feedbackmid", //Delay section -> Mid
                                       "sendside", "timeside", "lfospeedside", "lfodepthside", "feedbackside" };//Delay section -> Side
    for (int i = 0; i <= 19; i++)
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

    // Filter Modules initialization                    << Like filters here

    MidFilterModule.reset();
    SideFilterModule.reset();
    MidFilterModule.prepare(spec);                   
    SideFilterModule.prepare(spec);

    // Delay Modules Initializiation                    << Delays here and so on...

    MidDelayModule.reset();
    SideDelayModule.reset();
    MidDelayModule.prepare(spec);
    SideDelayModule.prepare(spec);

    // Low pass filter initialization -> will be used for filtering out big changes in delay time

    MidSampler.reset();
    SideSampler.reset();
    MidSampler.prepare(spec);
    SideSampler.prepare(spec);

    //SmoothedValues -> Creates linear interpolation in parameter changes.
    
    float rampTime = 0.02;

        //GUI

    Width_Target.reset(sampleRate, rampTime);

    Time_Mid_Target.reset(sampleRate, rampTime);
    Time_Side_Target.reset(sampleRate, rampTime);

    LFO_Depth_Mid_Target.reset(sampleRate, rampTime);
    LFO_Depth_Side_Target.reset(sampleRate, rampTime);
    LFO_Speed_Mid_Target.reset(sampleRate, rampTime);
    LFO_Speed_Side_Target.reset(sampleRate, rampTime);
        
        //Others

    lfoValueMid_Target.reset(sampleRate, rampTime);
    lfoValueSide_Target.reset(sampleRate, rampTime);
 
    
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

void MSUtilityAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i) // Clears channels from trash data
        buffer.clear (i, 0, buffer.getNumSamples());
    {

        for (int channel = 0; channel < totalNumInputChannels; ++channel) // Outer Loop handles channel
        {
            auto* channelData = buffer.getWritePointer(channel);
            
            auto* channelDataLeft = buffer.getWritePointer(0);
            auto* channelDataRight = buffer.getWritePointer(1);

            for (int sample = 0; sample < buffer.getNumSamples(); ++sample) // Inner loop handles samples within channel
            {               

                float twoPi = juce::MathConstants<float>::twoPi; // stores twopi value

                // LFOs
                const auto LFO_Speed_Mid = LFO_Speed_Mid_Target.getNextValue();
                const auto LFO_Speed_Side = LFO_Speed_Side_Target.getNextValue();
                const auto LFO_Depth_Mid = LFO_Depth_Mid_Target.getNextValue();
                const auto LFO_Depth_Side = LFO_Depth_Side_Target.getNextValue();
                
                //LFO Phase Calculation <- needed for LFOs
             
                float twoPie = juce::MathConstants<float>::twoPi; // stores two * pi value

                if (updateCounter == updateRate) // Updates once per 100 (updateRate) samples
                {
                    lfoPhaseMid += twoPie * LFO_Speed_Mid * updateRate / getSampleRate();   // Phase goes from 0 to twoPi
                    lfoPhaseSide += twoPie * LFO_Speed_Side * updateRate / getSampleRate(); // Once every Speed / sampleRate
                                                                                            // * by updateRate ensures = result after counter
                    if (lfoPhaseMid > twoPie)        // This ensures lfoPhaseMid/Side 
                        lfoPhaseMid -= twoPie;       // does not increase indefinitely

                    if (lfoPhaseSide > twoPie)
                        lfoPhaseSide -= twoPie;

                    lfoValueMid_Target.setTargetValue(LFO_Depth_Mid * std::sin(lfoPhaseMid));      // lfoPhase values are introduced into sin() - and ramped -
                    lfoValueSide_Target.setTargetValue(LFO_Depth_Side * std::sin(lfoPhaseSide));   // returning a value from -1 to 1, scaled to LFO_Depth
                                                                                                   // to -1000 to 1000 (max)
                    updateCounter = 0;              //ensures counter re-starts
                }

                else
                {
                    updateCounter++;                // counter... counts (once per sample)
                }

                lfoValueMid = lfoValueMid_Target.getNextValue();                // lfo values are extracted
                lfoValueSide = lfoValueSide_Target.getNextValue();

                //Time Modulation

                Time_Side = Time_Side_Target.getNextValue() + lfoValueSide;     // Add LFO to (ramped) Time_Side
                Time_Mid = Time_Mid_Target.getNextValue() + lfoValueMid;        // Add LFO to (ramped) Time_Mid

                Time_Side = SideSampler.processSample(channel, Time_Side);      // Extra interpolation for Time_Mid
                Time_Mid = MidSampler.processSample(channel, Time_Mid);         // and Time_Side with low-pass filter

               // Regulator  

               if (Time_Side < 0)           // Keeps modulation oscillating
               {                            // smoothly instead
               Time_Side = -Time_Side;      // of stalling at 0
               }

               if (Time_Mid < 0)
               {
               Time_Mid = -Time_Mid;
               }              
               
               // Delay values are finally updated

               SideDelayModule.setDelay(Time_Side);
               MidDelayModule.setDelay(Time_Mid);

                        
               // Mid/Side encoding and Stereo Widening 

               Width = Width_Target.getNextValue(); // gets value from ramp

               if (Input_Type == "Stereo")
               {
                   xMidRaw = (2 - Width) * (channelDataLeft[sample] + channelDataRight[sample]);
                   xSideRaw = Width * (channelDataLeft[sample] - channelDataRight[sample]);
               }

               // Or Simply Mid/Side Mixer if input is Mid/Side

               else
               {
                   xMidRaw = channelDataLeft[sample] * (2 - Width);
                   xSideRaw = channelDataRight[sample] * (Width);
               }

               // Volume Control

               xMidRaw *= 0.5;
               xSideRaw *= 0.5;


               // Filtering

               const float xMidFilt = MidFilterModule.processSample(channel, xMidRaw);
               const float xSideFilt = SideFilterModule.processSample(channel, xSideRaw);

               // Delay

                    //Mid
                
               float dry = xMidFilt;                                                      // We take the filtered signal
               float wet = MidDelayModule.popSample(channel, Time_Mid);                   // Extract a delayed sample (read)
               MidDelayModule.pushSample(channel, xMidFilt + (wet * Feedback_Mid));       // Introduce a Sample into the delay module (write) - reintroducing the wet signal creates feedback
               float xMid = (dry * (Send_Mid - 1)) + (wet * Send_Mid);                    // Here the dry and wet signals are mixed

                    //Side

                dry = xSideFilt;
                wet = SideDelayModule.popSample(channel, Time_Side);
                SideDelayModule.pushSample(channel, xSideFilt + (wet * Feedback_Side));
                float xSide = (dry * (Send_Side - 1)) + (wet * Send_Side);

                // Output Handling

                if (Output_Type == "Stereo")
                { 

                    // Decoding Back to Stereo
                    newLeft = xMid + xSide;                                                                 
                    newRight = xMid - xSide;

                    // Pass decoded samples to channels

                    if (Input_Type == "Stereo") // If Stereo i/o -> Volume control
                    {
                        float volumeScale;

                        if (Width <= 1.f)
                        {
                            volumeScale = juce::jmap(Width, 1.0f, 0.0f, 0.0f, -6.0f);
                        }
                        else
                        {
                            volumeScale = juce::jmap(Width, 1.0f, 0.0f, 0.0f, 4.f);
                        }

                        channelDataLeft[sample] = newLeft * juce::Decibels::decibelsToGain(volumeScale);
                        channelDataRight[sample] = newRight * juce::Decibels::decibelsToGain(volumeScale);                     
                    }

                    else
                    {
                    channelDataLeft[sample] = newLeft;
                    channelDataRight[sample] = newRight;
                    }
                    
                }
                else // output == mid/side
                {    // Channels Are Left in Mid and Side                                     
                    channelDataLeft[sample] = xMid;
                    channelDataRight[sample] = xSide;
                }
            }
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

    // Width and Input Section

    if (parameterID == "stereowidth")

        Width_Target.setTargetValue(newValue);       

    else if (parameterID == "input")
    {
        switch ((int)newValue) // Something happens to Input_Type
        {
        case 0:
              Input_Type = "Stereo";
            break;
        case 1:
            Input_Type = "Mid/Side";
            break;
        }
    }

    else if (parameterID == "output")
    {
        switch ((int)newValue)
        {
        case 0:
            Output_Type = "Stereo";
        case 1:
            Output_Type = "Mid/Side";

        }
    }

    //Filter Section

        //Mid

    else if (parameterID == "cutoffmid")
    { 
        Cut_Off_Mid = newValue;
        MidFilterModule.setCutoffFrequency(Cut_Off_Mid);
    }

    else if (parameterID == "modemid")
    {
        switch ((int)newValue) // Something happens to Input_Type
        {
        case 0:
            MidFilterModule.setType(juce::dsp::StateVariableTPTFilterType::lowpass);
            break;
        case 1:
            MidFilterModule.setType(juce::dsp::StateVariableTPTFilterType::bandpass);
            break;
        case 2:
            MidFilterModule.setType(juce::dsp::StateVariableTPTFilterType::highpass);
            break;
        }
    }

    else if (parameterID == "resonancemid")
    {
        MidFilterModule.setResonance(newValue);
    }
 

        //Side

    else if (parameterID == "cutoffside")
    {
        Cut_Off_Side = newValue;
        SideFilterModule.setCutoffFrequency(Cut_Off_Side);
    }

    else if (parameterID == "modeside")
    {
        switch ((int)newValue) // Something happens to Input_Type
        {
        case 0:
            SideFilterModule.setType(juce::dsp::StateVariableTPTFilterType::lowpass);
            break;
        case 1:
            SideFilterModule.setType(juce::dsp::StateVariableTPTFilterType::bandpass);
            break;
        case 2:
            SideFilterModule.setType(juce::dsp::StateVariableTPTFilterType::highpass);
            break;
        }
    }

    else if (parameterID == "resonanceside")
    {
        SideFilterModule.setResonance(newValue);
    }


    //Delay Section


        //Mid
    else if (parameterID == "sendmid")
    {
        Send_Mid = newValue;
    }
    else if (parameterID == "timemid")
    {
                                      
        Time_Mid_Target.setTargetValue(newValue);

      //  Time_Mid = newValue;
      //  MidDelayModule.setDelay(newValue);
    }
    else if (parameterID == "lfospeedmid")
    {
        LFO_Speed_Mid_Target.setTargetValue(newValue);
    }
    else if (parameterID == "lfodepthmid")
    {
        LFO_Depth_Mid_Target.setTargetValue(newValue);
    }
    else if (parameterID == "feedbackmid")
    {
        Feedback_Mid = newValue;
    }
        //Side
    else if (parameterID == "sendside")
    {
        Send_Side = newValue;
    }
    else if (parameterID == "timeside")
    {
        Time_Side_Target.setTargetValue(newValue);

    }
    else if (parameterID == "lfospeedside")
    {
        LFO_Speed_Side_Target.setTargetValue(newValue);
    }
    else if (parameterID == "lfodepthside")
    {
        LFO_Depth_Side_Target.setTargetValue(newValue);
    }
    else if (parameterID == "feedbackside")
    {
        Feedback_Side = newValue;
    }
}


//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MSUtilityAudioProcessor();
}
