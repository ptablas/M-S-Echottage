/*
==============================================================================
Oscilloscope.h
Created : 26 Jan 2016 1 : 06 : 47am
Author : Geri
==============================================================================
*/

#include "JuceHeader.h"

#ifndef OSCILLOSCOPE_H_INCLUDED
#define OSCILLOSCOPE_H_INCLUDED
#include <array>


class Oscilloscope : public juce::Component, private juce::Timer
{
public:
    //==========================================================================
    Oscilloscope()
        : writePos(0)
    {
        startTimer(60);
    }
    //==========================================================================
    void pushBuffer(const float* data, int numSamples)
    {
        for (int i = 0; i < numSamples; ++i)
            buffer[++writePos % buffer.size()] = data[i];
    }
    //==========================================================================
    void paint(juce::Graphics& g) override
    {
        g.fillAll(juce::Colours::black.brighter(0.22f));
        juce::Rectangle<int> r = getLocalBounds();
        juce::Path path;
        path.startNewSubPath(0, 0.5 * r.getHeight());
        const float bufferYscale = 1.0f;
        int paintPos = 2;
        while (paintPos < buffer.size())
        {
            if (isZeroCrossing(paintPos))
                break;
            ++paintPos;
        }
        const int posOffset = paintPos;
        while (paintPos < buffer.size())
        {
            juce::Point<float> p((paintPos - posOffset) * r.getWidth() / paintSize,
                0.5 * ((bufferYscale * buffer[paintPos]) + 1) * r.getHeight());

            path.lineTo(p);
            ++paintPos;
        }
        g.setColour(juce::Colour(0, 102, 204));
        g.strokePath(path, juce::PathStrokeType(1.5f));
    }
private:
    //==========================================================================
    void timerCallback() override
    {
        repaint();
    }
    bool isZeroCrossing(int i) const noexcept
    {
        jassert(i > 0);
        return buffer[i] > buffer[i - 1] && buffer[i] > 0 && buffer[i - 1] < 0;
    }
    //==========================================================================
    std::array<float, 1024> buffer;
    std::size_t writePos;
    const int bufferSize = 16384;
    const int paintSize = 256;
};


#endif  // OSCILLOSCOPE_H_INCLUDED