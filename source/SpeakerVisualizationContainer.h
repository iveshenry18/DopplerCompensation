//
// Created by Henry Ives on 4/9/24.
//

#pragma once
#include "DopplerSpinner.h"
#include "PhysicalToViewport.h"
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

private:
    void drawGridlines (juce::Graphics& g);

    juce::Rectangle<float> mSpeaker;
    juce::Rectangle<float> mPhantomSpeaker;
    juce::Rectangle<float> mOrigin;
    juce::Rectangle<float> mFocalPoint;
    PhysicalToViewport mPhysicalToViewport;
    DopplerSpinner* mDopplerSpinner;
};
