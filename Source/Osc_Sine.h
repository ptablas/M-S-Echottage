/*
  ==============================================================================

    LFO.h
    Created: 30 May 2023 2:15:35pm
    Author:  pablo

  ==============================================================================
*/

#pragma once
#include <cmath>
#include <JuceHeader.h>

class Osc_Sine
{
    private:
        const double twoPie = 6.28318530718; // stores two * pi value
        double m_sampleRate;
        double m_phase, m_speed, m_depth;

    public:
        
        Osc_Sine()
            : m_phase(0), m_speed(0), m_depth(0)
        {
        }

        Osc_Sine(double sR)
            : m_phase(0), m_speed(0), m_depth(0), m_sampleRate(sR)
        {
        }

        void prepare(double sR);
        
        double output(double speed, double depth); 
};