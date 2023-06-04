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

    m_phase += twoPie * m_speed / m_sampleRate;

    if (m_phase > twoPie)
        m_phase -= twoPie;


    switch (m_waveform)
    {
        case Sine:
            
            out = std::sin(m_phase);

            break;

        case Triangle:

            if (m_phase <= Pie)
            {
                out = m_phase / Pie;
            }
            
            else
            {
                out = (m_phase / Pie) - 1;
            }

            break;

        case Sawtooth:

            out = m_phase / twoPie;

            break;

        case Square:

            if (m_phase >= Pie)
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

            if (m_phase == 0)
            {
                out = (((rand() % 200000) - 100000) / static_cast<double> (100000));
            }

            else

            break;
    }
    
    return (out * m_depth);
}


