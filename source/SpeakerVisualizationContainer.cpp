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
    physicalToViewport.setViewportBounds (getBounds());
}

void SpeakerVisualizationContainer::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colours::whitesmoke);
    SpinnerState spinnerState = mDopplerSpinner->getNextState (false);
    physicalToViewport.setPhysicalBounds (juce::Rectangle<float> {}.withCentre ({ 0, 0 }).withSizeKeepingCentre (spinnerState.spinnerDiameter * 1.1, std::max (spinnerState.spinnerDiameter / 2, spinnerState.distanceToFocalPoint) * 2.2));
    physicalToViewport.setViewportBounds (getBounds());

    juce::Point<int> phantomPhantomSpeakerViewportPosition = physicalToViewport.transform (spinnerState.phantomSpeakerPosition);
    mPhantomSpeaker.setSize (6, 6);
    mPhantomSpeaker.setCentre (phantomPhantomSpeakerViewportPosition.getX(), phantomPhantomSpeakerViewportPosition.getY());
    g.setColour (juce::Colours::grey);
    g.fillEllipse (mPhantomSpeaker);

    juce::Point<int> speakerViewportPosition = physicalToViewport.transform (spinnerState.speakerPosition);
    mSpeaker.setSize (8, 8);
    mSpeaker.setCentre (speakerViewportPosition.getX(), speakerViewportPosition.getY());
    g.setColour (spinnerState.isClicking ? juce::Colours::red : juce::Colours::black);
    g.fillEllipse (mSpeaker);

    juce::Point<int> originViewportPosition = physicalToViewport.transform (juce::Point<float> { 0, 0 });
    mOrigin.setSize (3, 3);
    mOrigin.setCentre (originViewportPosition.getX(), originViewportPosition.getY());
    g.setColour (juce::Colours::black);
    g.fillEllipse (mOrigin);

    juce::Point<int> focalPointViewportPosition = physicalToViewport.transform (juce::Point<float> { 0, -spinnerState.distanceToFocalPoint });
    mFocalPoint.setSize (5, 5);
    mFocalPoint.setCentre (focalPointViewportPosition.getX(), focalPointViewportPosition.getY());
    g.setColour (juce::Colours::lightsalmon);
    g.fillEllipse (mFocalPoint);

    g.setColour (juce::Colours::grey);
    g.drawLine (juce::Line<int> { originViewportPosition.getX(), originViewportPosition.getY(), speakerViewportPosition.getX(), speakerViewportPosition.getY() }.toFloat());
}

void SpeakerVisualizationContainer::timerCallback()
{
    repaint();
}
