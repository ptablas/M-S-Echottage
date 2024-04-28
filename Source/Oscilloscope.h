#pragma once

#include <JuceHeader.h>

class Oscilloscope : public juce::Component
{
public:
    void popSample(int sample)
    {
        oscilloscopeData.push_back(sample);
        repaint();
    }

    void paint(juce::Graphics& g) override
    {
        const auto b = getLocalBounds().toFloat();
        const auto w = b.getWidth();
        const auto h = b.getHeight();

        const auto centre = b.getCentre();
        const auto horizontalCentre = centre.getX();
        const auto verticalCentre = centre.getY();

        const auto topRightCorner = b.getTopRight();

        const juce::ColourGradient backgroundGradient(juce::Colours::cyan, centre,
            juce::Colours::magenta, topRightCorner, true);
        // Create a linear gradient for the oscilloscope path
        const juce::ColourGradient lineGradient(juce::Colours::magenta, centre,
            juce::Colours::cyan, topRightCorner, true);

        // Set the gradient as the background
        const juce::Rectangle rectangle(b);

        g.setGradientFill(backgroundGradient);
        g.fillEllipse(rectangle);

        while (oscilloscopeData.size() > oscilloscopeResolution)
        {
            oscilloscopeData.pop_front();
        }


        g.saveState(); // Save the current state of the Graphics context

        // Define the center and radius of the circle
        const int centerX = getWidth() / 2;
        const int centerY = getHeight() / 2;
        const int radius = juce::jmin(getWidth(), getHeight()) / 2;

        // Create a Path that represents the circle
        juce::Path circlePath;

        circlePath.addCentredArc(centerX, centerY, radius, radius,
            0.0, 0.0, juce::MathConstants<float>::twoPi, true);

        // Use the Path as a clip region
        g.reduceClipRegion(circlePath);

        g.setGradientFill(lineGradient);

        float lineThickness = 4.5f; // Change this to your desired line thickness

        // Draw lines directly
        for (int i = 1; i < oscilloscopeData.size(); ++i)
        {
            const float scale = w / oscilloscopeData.size();
            const int mappedPixel = i * scale;
            const int previousMappedPixel = (i - 1) * scale;
            const float value = oscilloscopeData[i] + verticalCentre;
            const float previousValue = oscilloscopeData[i - 1] + verticalCentre;
            g.drawLine(previousMappedPixel, previousValue, mappedPixel, value, lineThickness);
        }

        g.restoreState(); // Restore the Graphics context to its original state
    }

private:
    std::deque<float> oscilloscopeData;
    int oscilloscopeResolution = 500; // Adjust this to your desired resolution
};
