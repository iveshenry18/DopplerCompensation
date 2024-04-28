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
    ~SpeakerVisualizationContainer() override;
    void paint (juce::Graphics& g) override;
    void resized() override;
    void timerCallback() override;

private:
    void drawGridlines (juce::Graphics& g);

    juce::Rectangle<float> mSpeakerPhysical;
    juce::Rectangle<float> mSpeakerViewport;
    juce::Rectangle<float> mPhantomSpeaker;
    juce::Rectangle<float> mCircumference;
    juce::Rectangle<float> mFocalPointHead;
    juce::Rectangle<float> mFocalPointBody;
    juce::Label mErrorMessage;
    PhysicalToViewport mPhysicalToViewport;
    DopplerSpinner* mDopplerSpinner;
};
