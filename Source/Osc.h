/*
  ==============================================================================

    Osc.h
    Created: 30 May 2023 2:15:35pm
    Author:  pablo

  ==============================================================================
*/

#pragma once
#include <cmath>
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
        const double Pie =    3.14159265359;
        const double twoPie = 6.28318530718;

        double m_sampleRate;
        double m_phase, m_speed, m_depth;
        Waveform m_waveform;
        bool m_sampler = 0;

    public:
        
        Osc()
            : m_phase(0), m_speed(0), m_depth(0), m_waveform(Sine)
        {
        }

        Osc(double sR)
            : m_phase(0), m_speed(0), m_depth(0), m_sampleRate(sR), m_waveform(Sine)
        {
        }

        Osc(Waveform waveform)
            : m_phase(0), m_speed(0), m_depth(0), m_waveform(waveform)
        {
        }

        Osc(double sR, Waveform waveform)
            : m_phase(0), m_speed(0), m_depth(0), m_waveform(waveform), m_sampleRate(sR)
        {
        }

        void prepare(double sR);

        void calculatePhase();
        
        void setWaveform(Waveform waveform);

        double output(double speed, double depth); 
};