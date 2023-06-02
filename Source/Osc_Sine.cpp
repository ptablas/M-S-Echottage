/*
  ==============================================================================

    LFO.cpp
    Created: 30 May 2023 2:15:36pm
    Author:  pablo

  ==============================================================================
*/

#include "Osc_Sine.h"

void Osc_Sine::prepare(double sR)
{
    m_sampleRate = sR;
}

double Osc_Sine::output(double speed, double depth)
{
    m_speed = speed;
    m_depth = depth;

    m_phase += twoPie * m_speed / m_sampleRate;

    if (m_phase > twoPie)
        m_phase -= twoPie;

    double out = m_depth * std::sin(m_phase);

    return out;
}
