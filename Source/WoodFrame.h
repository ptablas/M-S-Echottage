#pragma once

#include <JuceHeader.h>

class WoodFrame : public juce::Component
{
public:
    WoodFrame() {}

    void paint(juce::Graphics& g) override
    {
        // Define the center and radius of the circle
        const int centerX = getWidth() / 2;
        const int centerY = getHeight() / 2;
        const int radius = juce::jmin(getWidth(), getHeight()) / 2;

        const int cellSize = (int)(getWidth() * 0.025f); // Adjust as needed
        const auto darkBrown = juce::Colour::fromRGB(61, 43, 31); // A very deep, almost black, brown
        const auto oakBrown = juce::Colour::fromRGB(204, 119, 34); // A golden brown

        // Create a Path that represents the circle
        juce::Path circlePath;

        circlePath.addCentredArc(centerX, centerY, radius, radius,
            0.0, 0.0, juce::MathConstants<float>::twoPi, true);

        // Use the Path as a clip region
        g.reduceClipRegion(circlePath);

        for (int x = 0; x < getWidth(); x += cellSize)
        {
            for (int y = 0; y < getHeight(); y += cellSize)
            {
                const bool isEven = ((x / cellSize + y / cellSize) % 2 == 0);
                juce::Rectangle<int> cell(x, y, cellSize, cellSize);

                const auto cellX = cell.getX();
                const auto cellY = cell.getY();
                const auto cellR = cell.getRight();
                const auto cellB = cell.getBottom();

                const auto darkToOak = juce::ColourGradient(darkBrown, cellX, cellY,
                    oakBrown, cellR, cellB,
                    false);

                const auto oakToBrown = juce::ColourGradient(oakBrown, cellX, cellY,
                    darkBrown, cellR, cellB,
                    false);

                g.setGradientFill(isEven ? darkToOak : oakToBrown);
                g.fillRect(cell);
            }
        }
    }
};