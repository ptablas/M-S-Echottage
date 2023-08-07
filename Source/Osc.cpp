/*
  ===============================CPP============================================

  Author: Pablo Tablas

  Osc is an oscillator class containing a variety of waveforms (Sine, Triangle,
  Sawtooth, Square and Random), apart from Sample & Hold capabilities by sampling
  said Random waveform.

  Computation of oscillator values in relation to phase is heavily influenced by 
  Bela's Andrew McPherson's "C++ Real-Time Audio Programming with Bela" Series. 
  Their example is here simply extrapolated to cover most basic waveforms.

  Osc is JUCE-compatible. If being used in a JUCE program, sample rate can
  potentially be initialized appropriately by passing the juce::dsp::ProcessSpec
  spec onto the prepare function.

  The member variable m_sampleRate is, however, shared among all instances of the
  class. Meaning the sample rate needs to be passed only to one instance of it.
  Declaring it again will not results in more errors, but'll take unnecessary space.
  
  ==============================================================================
*/

#include "Osc.h"

double Osc::m_sampleRate;

void Osc::prepare(double sR)
{
    m_sampleRate = sR;
}

#ifdef JUCE_HEADER_INCLUDED
void Osc::prepare(const juce::dsp::ProcessSpec& spec)
{
    jassert(spec.sampleRate > 0);
    m_sampleRate = spec.sampleRate;
}
#endif

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

double Osc::output(double speed, double depth, float * input)
{
    m_speed = speed;
    m_depth = depth;
    m_in = *input;

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
            m_out = -1 + (2 * m_phase / M_PI);

        else
            m_out = -1 - (2 * m_phase / M_PI);

        break;

    case Sawtooth:

        m_out = m_phase / (2 * M_PI);

        break;

    case Square:

        if (m_phase > 0)
            m_out = 1;

        else
            m_out = -1;

        break;

    case Random:

        if (m_sampler == 1)
        {
            m_out = m_randomDouble();
            m_sampler = 0;
        }

        break;

    case SH:

        if (m_sampler == 1)
        {
            m_out = m_in;
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
    //generate seed
    std::random_device rd;
    std::seed_seq sd{rd(), rd(), rd(), rd()};
    
    //generate random double
    static std::mt19937 gen(sd);
    static std::uniform_real_distribution<double> dis(-1.0, 1.0);
    return dis(gen);
}