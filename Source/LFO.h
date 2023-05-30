/*
  ==============================================================================

    LFO.h
    Created: 30 May 2023 2:15:35pm
    Author:  pablo

  ==============================================================================
*/

#pragma once
#include <cmath>

class LFO
{
    public:
        double lfoPhase = 0;
        double output(double sampleRate, double lfoSpeed, double lfoDepth);

    private:   
        const double twoPie = 6.28318530718; // stores two * pi value
};