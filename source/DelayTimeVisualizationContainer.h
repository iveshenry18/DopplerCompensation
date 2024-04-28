//
// Created by Henry Ives on 4/28/24.
//

#pragma once
#include "DopplerSpinner.h"
#include "PluginProcessor.h"
#include <juce_audio_processors/juce_audio_processors.h>

class DelayTimeVisualizationContainer : public juce::Component, public juce::Timer
{
public:
    explicit DelayTimeVisualizationContainer (DopplerSpinner* dopplerSpinner);
    ~DelayTimeVisualizationContainer() override;
    void paint (juce::Graphics& g) override;
    void timerCallback() override;
private:
    DopplerSpinner* mDopplerSpinner;
    std::deque<float> delayTimes;
};
