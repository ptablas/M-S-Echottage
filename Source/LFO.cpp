/*
  ==============================================================================

    LFO.cpp
    Created: 30 May 2023 2:15:36pm
    Author:  pablo

  ==============================================================================
*/

#include "LFO.h"

double LFO::output(double sampleRate, double lfoSpeed, double lfoDepth)
{
    lfoPhase += twoPie * lfoSpeed / sampleRate;

    if (lfoPhase > twoPie)
        lfoPhase -= twoPie;

    double out = lfoDepth * std::sin(lfoPhase);

    return out;
}
