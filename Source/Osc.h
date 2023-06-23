/*
  ==============================================================================

    Osc.h
    Created: 30 May 2023 2:15:35pm
    Author:  pablo

  ==============================================================================
*/

#pragma once

#define _USE_MATH_DEFINES
#include <cmath>
#include <random>
#include <JuceHeader.h>


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

        double m_sampleRate;
        double m_phase, m_speed, m_depth, m_out;
        Waveform m_waveform;
        bool m_sampler = 0;
        
        void m_calculatePhase();
        double m_randomDouble();
        void m_waveSwitch();

    public:
        
        Osc()
            : m_out(0), m_phase(0), m_speed(0), m_depth(0), m_waveform(Sine), m_sampleRate(0)
        {
        }

        Osc(double sR)
            : m_out(0), m_phase(0), m_speed(0), m_depth(0), m_waveform(Sine), m_sampleRate(sR)
        {
        }

        Osc(Waveform waveform)
            : m_out(0), m_phase(0), m_speed(0), m_depth(0), m_waveform(waveform), m_sampleRate(0)
        {
        }

        Osc(double sR, Waveform waveform)
            : m_out(0), m_phase(0), m_speed(0), m_depth(0), m_waveform(waveform), m_sampleRate(sR)
        {
        }
 
        void prepare(double sR);
        
        void setWaveform(Waveform waveform);

        double output(double speed, double depth); 
};