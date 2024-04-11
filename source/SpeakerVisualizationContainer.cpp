//
// Created by Henry Ives on 4/9/24.
//

#include "SpeakerVisualizationContainer.h"

SpeakerVisualizationContainer::SpeakerVisualizationContainer (DopplerSpinner* dopplerSpinner) : mDopplerSpinner (dopplerSpinner)
{
    startTimer (16);
}

SpeakerVisualizationContainer::~SpeakerVisualizationContainer()
{
    stopTimer();
}

void SpeakerVisualizationContainer::resized()
{
}

void SpeakerVisualizationContainer::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colours::lightskyblue);
    SpinnerState spinnerState = mDopplerSpinner->getNextState (false);
    float spinnerRadius = spinnerState.diameter * 5;
    juce::Rectangle<float> physicalBounds = juce::Rectangle (float (spinnerState.diameter * 1.1), float ((spinnerRadius * 1.1) + (spinnerState.distanceToFocalPoint * 1.1)));
    juce::Point<float> physicalOrigin = juce::Point (float (physicalBounds.getCentreX()), float (spinnerRadius * 1.1));

    juce::AffineTransform physicalToViewport = juce::AffineTransform {}
                                                   // Rotate 180 degrees about the origin
                                                   .rotated (M_PI)
                                                   // Move center of physicalBound to 0,0
                                                   .translated (0, physicalOrigin.getY() - (physicalBounds.getCentreY()))
                                                   // Scale about 0,0 by either the height ratio or width ratio—whichever's smaller
                                                   .scaled (std::min (getWidth() / physicalBounds.getWidth(), getHeight() / physicalBounds.getHeight()))
                                                   // Move 0,0 (physicalBound center) to center of viewport
                                                   .translated (getWidth() / 2, getHeight() / 2);

    juce::Point<int>
        speakerViewportPosition = spinnerState.speakerPosition.transformedBy (physicalToViewport).roundToInt();
    mSpeaker.setSize (12, 12);
    mSpeaker.setPosition (speakerViewportPosition.getX(), speakerViewportPosition.getY());
    g.setColour (spinnerState.isClicking ? juce::Colours::green : juce::Colours::red);
    g.fillEllipse (mSpeaker);

//    juce::Rectangle<int> physicalBoundsViewportPosition = physicalBounds.translated (-physicalOrigin.getX(), -physicalOrigin.getY()).transformedBy (physicalToViewport).toNearestIntEdges();
//    g.setColour (juce::Colours::lightblue);
//    g.drawRect (physicalBoundsViewportPosition);

    juce::Point<int> originViewportPosition = juce::Point<float> { 0, 0 }.transformedBy (physicalToViewport).roundToInt();
    mOrigin.setSize (5, 5);
    mOrigin.setPosition (originViewportPosition.getX(), originViewportPosition.getY());
    g.setColour (juce::Colours::black);
    g.fillEllipse (mOrigin);

    juce::Point<int> focalPointViewportPosition = juce::Point<float> { 0, -spinnerState.distanceToFocalPoint }.transformedBy (physicalToViewport).roundToInt();
    mFocalPoint.setSize (8, 8);
    mFocalPoint.setPosition (focalPointViewportPosition.getX(), focalPointViewportPosition.getY());
    g.setColour (juce::Colours::white);
    g.fillEllipse (mFocalPoint);
}

void SpeakerVisualizationContainer::timerCallback()
{
    repaint();
}
