#pragma once

#include <JuceHeader.h>

class CheckBackground : public juce::Component
{
public:
    CheckBackground() {}

    void paint(juce::Graphics& g) override
    {
        const int cellSize = (int) (getWidth() * 0.1f); // Adjust as needed
        const auto black = juce::Colour::fromRGB(15, 12, 41);
        const auto white = juce::Colours::antiquewhite;

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

                const auto blackToWhite = juce::ColourGradient(black, cellX, cellY,
                    white, cellR, cellB,
                    false);

                const auto whiteToBlack = juce::ColourGradient(white, cellX, cellY,
                    black, cellR, cellB,
                    false);

                g.setGradientFill(isEven ? blackToWhite : whiteToBlack);
                g.fillRect(cell);
            }
        }
    }
};