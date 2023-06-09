/*
  ==============================================================================

    Osc.cpp
    Created: 30 May 2023 2:15:36pm
    Author:  pablo

  ==============================================================================
*/

#include "Osc.h"

double Osc::m_sampleRate;

void Osc::prepare(double sR)
{
    m_sampleRate = sR;
}

void Osc::prepare(const juce::dsp::ProcessSpec& spec)
{
    jassert(spec.sampleRate > 0);
    m_sampleRate = spec.sampleRate;
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
            m_out = -1 + (2 * m_phase / M_PI);
        }

        else
        {
            m_out = -1 - (2 * m_phase / M_PI);
        }

        break;

    case Sawtooth:

        m_out = m_phase / (2 * M_PI);

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
    m_phase += (2 * M_PI) * m_speed / m_sampleRate;

    if (m_phase > M_PI)
    {
        m_phase -= (2 * M_PI);
        m_sampler = 1;
    }
}

double Osc::m_randomDouble()
{
    static std::mt19937 gen(0);
    static std::uniform_real_distribution<double> dis(-1.0, 1.0);
    return dis(gen);
}