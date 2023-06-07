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

    m_waveSwitch();
    m_calculatePhase();

    return (m_out * m_depth);
}

void Osc::m_waveSwitch()
{
    switch (m_waveform)
    {
    case Sine:

        m_out = std::sin(m_phase);

        break;

    case Triangle:

        if (m_phase > 0)
        {
            m_out = -1 + (2 * m_phase / Pie);
        }

        else
        {
            m_out = -1 - (2 * m_phase / Pie);
        }

        break;

    case Sawtooth:

        m_out = m_phase / twoPie;

        break;

    case Square:

        if (m_phase > 0)
        {
            m_out = 1;
        }

        else
        {
            m_out = -1;
        }

        break;

    case Random:

        m_out = m_randomDouble();

        break;

    case SH:

        if (m_sampler == 1)
        {
            m_out = m_randomDouble();

            m_sampler = 0;
        }

        break;
    }
}

void Osc::m_calculatePhase()
{
    m_phase += twoPie * m_speed / m_sampleRate;

    if (m_phase > Pie)
    {
        m_phase -= twoPie;
        m_sampler = 1;
    }
}

double Osc::m_randomDouble()
{
    std::random_device rd;  // Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()
    std::uniform_real_distribution<> dis(-1.0, 1.0);

    return dis(gen);
}

double LFO::output(double speed, double depth)
{
    m_speed = speed;
    m_depth = depth;

    m_downsample += m_speed * 2 / m_sampleRate;


    if (m_downsample >= m_speed * 2)
    {

        m_waveSwitch();
        m_calculatePhase();

        m_downsample = 0;
    }

    return (m_out * m_depth);

}

void LFO::m_calculatePhase()
{
    m_phase += twoPie * m_speed * (m_speed * 2) / m_sampleRate;

    if (m_phase > Pie)
    {
        m_phase -= twoPie;
        m_sampler = 1;
    }
}
