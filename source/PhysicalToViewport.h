//
// Created by Henry Ives on 4/12/24.
//

#pragma once
#include "PluginProcessor.h"
#include <juce_audio_processors/juce_audio_processors.h>

class PhysicalToViewport
{
public:
    PhysicalToViewport (juce::Rectangle<float> physicalBounds, juce::Rectangle<int> viewportBounds);
    PhysicalToViewport();
    juce::Point<int> transform (juce::Point<float> in);
    juce::Rectangle<int> transform (juce::Rectangle<float> in);
    void setPhysicalBounds (juce::Rectangle<float> physicalBounds)
    {
        mPhysicalBounds = physicalBounds;
        updateExpandedPhysicalBounds();
    }
    void setViewportBounds (juce::Rectangle<int> viewportBounds)
    {
        mViewportBounds = viewportBounds;
        updateExpandedPhysicalBounds();
    }

    juce::Rectangle<float> getExpandedPhysicalBounds()
    {
        return mExpandedPhysicalBounds;
    }

private:
    void updateExpandedPhysicalBounds();
    juce::Rectangle<float> mPhysicalBounds;
    juce::Rectangle<float> mExpandedPhysicalBounds;
    juce::Rectangle<int> mViewportBounds;
};
