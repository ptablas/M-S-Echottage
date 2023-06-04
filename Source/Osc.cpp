/*
  ==============================================================================

    Osc.cpp
    Created: 30 May 2023 2:15:36pm
    Author:  pablo

  ==============================================================================
*/

#include "Osc.h"

void Osc::prepare(double sR)
{
    m_sampleRate = sR;
}

void Osc::setWaveform(Waveform waveform)
{
    m_waveform = waveform;
}

double Osc::output(double speed, double depth)
{
    m_speed = speed;
    m_depth = depth;
    double out;

    switch (m_waveform)
    {
        case Sine:
            
            m_phase += twoPie * m_speed / m_sampleRate;

            if (m_phase > twoPie)
                m_phase -= twoPie;

            out = m_depth * std::sin(m_phase);
            break;

        case Triangle:



            break;
        case Sawtooth:
            break;
        case Square:
            break;
        case Random:
            break;
    }
    
    return out;
}


