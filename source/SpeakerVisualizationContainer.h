//
// Created by Henry Ives on 4/9/24.
//

#pragma once
#include "DopplerSpinner.h"
#include "PluginProcessor.h"
#include <juce_audio_processors/juce_audio_processors.h>

class SpeakerVisualizationContainer : public juce::Component, public juce::Timer
{
public:
    SpeakerVisualizationContainer (DopplerSpinner* dopplerSpinner);
    ~SpeakerVisualizationContainer();
    void paint (juce::Graphics& g) override;
    void resized() override;
    void timerCallback() override;
    juce::Point<int> physicalToViewport (juce::Point<float> in, juce::Rectangle<float> physicalBounds = juce::Rectangle<float> {}.withCentre ({ 0, 0 }).withSizeKeepingCentre (MAX_SPINNER_DIAMETER * 1.1, std::max (MAX_SPINNER_DIAMETER / 2, MAX_DISTANCE_TO_FOCAL_POINT) * 2.2));

private:
    juce::Rectangle<float> mSpeaker;
    juce::Rectangle<float> mOrigin;
    juce::Rectangle<float> mFocalPoint;
    DopplerSpinner* mDopplerSpinner;
};
