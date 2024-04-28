//
// Created by Henry Ives on 4/28/24.
//

#include "DelayTimeVisualizationContainer.h"

DelayTimeVisualizationContainer::DelayTimeVisualizationContainer (DopplerSpinner* dopplerSpinner) : mDopplerSpinner (dopplerSpinner)
{
    startTimerHz (1000);
}
DelayTimeVisualizationContainer::~DelayTimeVisualizationContainer()
{
    stopTimer();
}

void DelayTimeVisualizationContainer::timerCallback()
{
    repaint();
}

void DelayTimeVisualizationContainer::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colours::whitesmoke);
    SpinnerState spinnerState = mDopplerSpinner->getCurrentState();
    delayTimes.push_back (spinnerState.timeDeltaSeconds);
    if (delayTimes.size() > getWidth())
    {
        delayTimes.pop_front();
    }
    auto maxDelayTime = *std::max_element (delayTimes.begin(), delayTimes.end());

    juce::Path path;
    for (int i = 0; i < delayTimes.size(); i++)
    {
        auto mappedDelayTime = juce::jmap (delayTimes.at (i), 0.f, maxDelayTime, (float) (getHeight() * 0.9), (float) (getHeight() * 0.1));
        path.lineTo ((float) i, mappedDelayTime);
    }
    g.setColour (juce::Colours::darkblue);
    g.strokePath (path, juce::PathStrokeType (1.0f));
}