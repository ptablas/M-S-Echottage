/*
  ===============================HEADER=========================================

  Author: Pablo Tablas

  Osc is an oscillator class containing a variety of waveforms (Sine, Triangle,
  Sawtooth, Square and Random), apart from Sample & Hold capabilities by sampling
  said Random waveform.

  Computation of oscillator values in relation to phase is heavily influenced by
  Bela's Andrew McPherson's "C++ Real-Time Audio Programming with Bela" Series.
  Their example is here simply extrapolated to cover most basic waveforms.

  Osc is JUCE-compatible. If being used within the JUCE framework, sample rate can
  potentially be initialized appropriately by passing the juce::dsp::ProcessSpec
  spec onto the prepare function.
  
*/


// COMMENT THIS LINE IF NOT being used within the JUCE framework
#define JUCE_HEADER_INCLUDED 

/*

  The member variable m_sampleRate is, however, shared among all instances of the
  class. Meaning the sample rate needs to be passed only to one instance of it.
  Declaring it again will not results in more errors, but'll take unnecessary space.

  ==============================================================================
*/

#pragma once

#define _USE_MATH_DEFINES
#include <cmath>
#include <random>

#ifdef JUCE_HEADER_INCLUDED
#include <JuceHeader.h>
#endif  


class Osc
{
    public:

        enum Waveform
        {
            Sine = 0,
            Triangle,
            Square,
            Sawtooth,
            Random,
            SH
        };

private:

        static double m_sampleRate;
        double m_phase, m_speed, m_depth, m_out, m_in;
        Waveform m_waveform;
        bool m_sampler = 0;
        
        void m_calculatePhase();
        double m_randomDouble();
        void m_waveSwitch();

    public:
        
        Osc()
            : m_out(0), m_phase(0), m_speed(0), m_depth(0), m_waveform(Sine)
        {
            m_sampleRate = 0;
        }

        Osc(double sR)
            : m_out(0), m_phase(0), m_speed(0), m_depth(0), m_waveform(Sine)
        {
            m_sampleRate = sR;
        }

        Osc(Waveform waveform)
            : m_out(0), m_phase(0), m_speed(0), m_depth(0), m_waveform(waveform)
        {
            m_sampleRate = 0;
        }

        Osc(double sR, Waveform waveform)
            : m_out(0), m_phase(0), m_speed(0), m_depth(0), m_waveform(waveform)
        {
            m_sampleRate = sR;
        }
 
        void prepare(double sR);

        #ifdef JUCE_HEADER_INCLUDED
        void prepare(const juce::dsp::ProcessSpec& spec);
        #endif  
        
        void setWaveform(Waveform waveform);

        double output(double speed, double depth);
        double output(double speed, double depth, float * input);
};