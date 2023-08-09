/*
  ==============================================================================

    Oscilloscope.h
    Created: 9 Aug 2023 3:11:24pm
    Author:  pablo

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

class Oscilloscope : public juce::AudioVisualiserComponent
{
public:
    Oscilloscope() : AudioVisualiserComponent(1)
    {
        setBufferSize(512);
        setSamplesPerBlock(256);
        setRepaintRate(30);
    }
};