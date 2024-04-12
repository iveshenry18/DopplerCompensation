//
// Created by Henry Ives on 4/9/24.
//

#include "SpeakerVisualizationContainer.h"

SpeakerVisualizationContainer::SpeakerVisualizationContainer (DopplerSpinner* dopplerSpinner) : mDopplerSpinner (dopplerSpinner)
{
    // ~60 fps
    startTimerHz (60);
}

SpeakerVisualizationContainer::~SpeakerVisualizationContainer()
{
    stopTimer();
}

void SpeakerVisualizationContainer::resized()
{
}

juce::Point<int> SpeakerVisualizationContainer::physicalToViewport (juce::Point<float> in, juce::Rectangle<float> physicalBounds)
{
    float viewportAspectRatio = getBounds().toFloat().getAspectRatio();
    float physicalAspectRatio = physicalBounds.getAspectRatio();
    float aspectRatioRatio = physicalAspectRatio / viewportAspectRatio;
    /**
     * The physicalBounds expanded to match the aspect ratio of the viewport
     */
    juce::Rectangle<float> expandedBounds = aspectRatioRatio < 1
                                                ? physicalBounds.withSizeKeepingCentre (physicalBounds.getWidth() / aspectRatioRatio, physicalBounds.getHeight())
                                                : physicalBounds.withSizeKeepingCentre (physicalBounds.getWidth(), physicalBounds.getHeight() * aspectRatioRatio);
    return juce::Point<float> {
        juce::jmap (in.getX(), expandedBounds.getX(), expandedBounds.getRight(), 0.f, float (getWidth())),
        juce::jmap (in.getY(), expandedBounds.getY(), expandedBounds.getBottom(), float (getHeight()), 0.f)
    }
        .roundToInt();
}

void SpeakerVisualizationContainer::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colours::lightskyblue);
    SpinnerState spinnerState = mDopplerSpinner->getNextState (false);

    juce::Point<int> speakerViewportPosition = physicalToViewport (spinnerState.speakerPosition);
    mSpeaker.setSize (12, 12);
    mSpeaker.setCentre (speakerViewportPosition.getX(), speakerViewportPosition.getY());
    g.setColour (spinnerState.isClicking ? juce::Colours::green : juce::Colours::red);
    g.fillEllipse (mSpeaker);

    juce::Point<int> originViewportPosition = physicalToViewport (juce::Point<float> { 0, 0 });
    mOrigin.setSize (5, 5);
    mOrigin.setCentre (originViewportPosition.getX(), originViewportPosition.getY());
    g.setColour (juce::Colours::black);
    g.fillEllipse (mOrigin);

    juce::Point<int> focalPointViewportPosition = physicalToViewport (juce::Point<float> { 0, -spinnerState.distanceToFocalPoint });
    mFocalPoint.setSize (8, 8);
    mFocalPoint.setCentre (focalPointViewportPosition.getX(), focalPointViewportPosition.getY());
    g.setColour (juce::Colours::white);
    g.fillEllipse (mFocalPoint);
}

void SpeakerVisualizationContainer::timerCallback()
{
    repaint();
}
