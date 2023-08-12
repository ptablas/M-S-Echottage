/*
  ==============================================================================

    Oscilloscope.h
    Created: 9 Aug 2023 3:11:24pm
    Author:  pablo

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

template <typename sampletype>
class OscilloscopeBuffer {
    
public:
    OscilloscopeBuffer();
    
    void setBufferSize(int bufferSize) { m_bufferSize = bufferSize; }
    void pushSample(sampletype sample) { m_samples.push_back(sample); };


private:
    int m_bufferSize;
    std::vector<sampletype> m_samples;

};

class Oscilloscope : juce::Component, 
                     juce::Timer {

public:


private:

};