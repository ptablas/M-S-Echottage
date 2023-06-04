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
    double out;
    m_speed = speed;
    m_depth = depth;

    switch (m_waveform)
    {
        case Sine:
            
            out = std::sin(m_phase);

            break;

        case Triangle:

            if (m_phase > 0)
            {
                out = -1 + (2 * m_phase / Pie);
            }
            
            else
            {
                out = -1 - (2 * m_phase / Pie);
            }

            break;

        case Sawtooth:

            out = m_phase / twoPie;

            break;

        case Square:

            if (m_phase > 0)
            {
                out = 1;
            }

            else
            {
                out = -1;
            }

            break;

        case Random:

            out = (((rand() % 200000) - 100000) / static_cast<double> (100000));

            break;

        case SH:

            if (m_sampler == 1)
            {
                out = (((rand() % 200000) - 100000) / static_cast<double> (100000));
                m_sampler = 0;
            }

            break;
    }
    
    calculatePhase();
    return (out * m_depth);
}


void Osc::calculatePhase()
{
    m_phase += twoPie * m_speed / m_sampleRate;

    if (m_phase > Pie)
    {
        m_phase -= twoPie;
        m_sampler = 1;
    }
}
