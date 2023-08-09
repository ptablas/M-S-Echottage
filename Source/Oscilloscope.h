/*
  ==============================================================================

    Oscilloscope.h
    Created: 9 Aug 2023 3:11:24pm
    Author:  pablo

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

class Oscilloscope : public juce::AudioVisualiserComponent
{
public:
    Oscilloscope() : AudioVisualiserComponent(1)
    {
        setBufferSize(30);
        setSamplesPerBlock(256);
        setRepaintRate(30);
    }

    void getChannelAsPath(juce::Path& path, const juce::Range<float>* levels,
        int numLevels, int nextSample)
    {
        path.preallocateSpace(4 * numLevels + 8);

        for (int i = 0; i < numLevels; ++i)
        {
            auto level = -(levels[(nextSample + i) % numLevels].getEnd());

            if (i == 0)
                path.startNewSubPath(0.0f, level);
            else

                if (i != i - 1)
                    path.lineTo((float)i, level); //learning about paths might solve this issue
                else
                    path.addStar(path.getCurrentPosition(), 1, 1, 1, 1);
        }

        for (int i = numLevels; --i >= 0;)
            path.lineTo((float)i, -(levels[(nextSample + i) % numLevels].getStart()));

        path.closeSubPath();
    }

    void paintChannel(juce::Graphics& g, juce::Rectangle<float> area,
        const juce::Range<float>* levels, int numLevels, int nextSample)
    {
        juce::Path p;
        getChannelAsPath(p, levels, numLevels, nextSample);

        g.fillPath(p, juce::AffineTransform::fromTargetPoints(0.f, -1.0f, area.getX(), area.getY(),
                                                              0.0f, 1.0f, area.getX(), area.getBottom(),
                                                              (float)numLevels, -1.0f, area.getRight(), area.getY()));
    }
};