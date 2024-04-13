//
// Created by Henry Ives on 4/9/24.
//

#include "SpeakerVisualizationContainer.h"

SpeakerVisualizationContainer::SpeakerVisualizationContainer (DopplerSpinner* dopplerSpinner) : mDopplerSpinner (dopplerSpinner)
{
    startTimerHz (120);
}

SpeakerVisualizationContainer::~SpeakerVisualizationContainer()
{
    stopTimer();
}

void SpeakerVisualizationContainer::resized()
{
    mPhysicalToViewport.setViewportBounds (getBounds());
}

void SpeakerVisualizationContainer::drawGridlines (juce::Graphics& g)
{
    g.setColour (juce::Colours::black);
    g.setOpacity (0.5);
    // Vertical axis
    juce::Rectangle<float> physicalTick;
    juce::Rectangle<int> viewportTick;
    for (int i = (int) mPhysicalToViewport.getExpandedPhysicalBounds().getX(); i <= (int) mPhysicalToViewport.getExpandedPhysicalBounds().getRight(); i++)
    {
        physicalTick = juce::Rectangle<float> { 0, 0 }.withCentre ({ (float) i, 0 }).withSizeKeepingCentre (0.1, 0.2);
        viewportTick = mPhysicalToViewport.transform (physicalTick);
        g.drawVerticalLine (viewportTick.getCentreX(), viewportTick.getY(), viewportTick.getBottom());
    }
    for (int i = (int) mPhysicalToViewport.getExpandedPhysicalBounds().getY(); i <= (int) mPhysicalToViewport.getExpandedPhysicalBounds().getBottom(); i++)
    {
        physicalTick = juce::Rectangle<float> { 0, 0 }.withCentre ({ 0, (float) i }).withSizeKeepingCentre (0.2, 0.1);
        viewportTick = mPhysicalToViewport.transform (physicalTick);
        g.drawHorizontalLine (viewportTick.getCentreY(), viewportTick.getX(), viewportTick.getRight());
    }
}

void SpeakerVisualizationContainer::paint (juce::Graphics& g)
{
    SpinnerState spinnerState = mDopplerSpinner->getNextState (false);
    g.fillAll (juce::Colours::whitesmoke);
    auto physicalBounds = juce::Rectangle<float> {}.withCentre ({ 0, 0 }).withSizeKeepingCentre (spinnerState.spinnerDiameter * 1.1, std::max (spinnerState.spinnerDiameter / 2, spinnerState.distanceToFocalPoint) * 2.2);
    mPhysicalToViewport.setPhysicalBounds (physicalBounds);
    mPhysicalToViewport.setViewportBounds (getBounds());

    drawGridlines (g);

    juce::Point<int> phantomPhantomSpeakerViewportPosition = mPhysicalToViewport.transform (spinnerState.phantomSpeakerPosition);
    mPhantomSpeaker.setSize (6, 6);
    mPhantomSpeaker.setCentre (phantomPhantomSpeakerViewportPosition.getX(), phantomPhantomSpeakerViewportPosition.getY());
    g.setColour (juce::Colours::lightgrey);
    g.fillEllipse (mPhantomSpeaker);

    mCircumference = juce::Rectangle<float> { 0, 0 }.withCentre ({ 0, 0 }).withSizeKeepingCentre (spinnerState.spinnerDiameter, spinnerState.spinnerDiameter);
    g.setColour (juce::Colours::lightgrey);
    g.drawEllipse (mPhysicalToViewport.transform (mCircumference).toFloat(), 1);

    juce::Point<int> speakerViewportPosition = mPhysicalToViewport.transform (spinnerState.speakerPosition);
    mSpeaker.setSize (10, 10);
    mSpeaker.setCentre (speakerViewportPosition.getX(), speakerViewportPosition.getY());
    g.setColour (juce::Colours::black);
    g.fillEllipse (mSpeaker);

    juce::Point<int> focalPointViewportPosition = mPhysicalToViewport.transform (juce::Point<float> { 0, -spinnerState.distanceToFocalPoint });
    mFocalPoint.setSize (10, 10);
    mFocalPoint.setCentre (focalPointViewportPosition.getX(), focalPointViewportPosition.getY());
    g.setColour (juce::Colours::lightsalmon);
    g.fillEllipse (mFocalPoint);

    juce::Point<int> originViewportPosition = mPhysicalToViewport.transform (juce::Point<float> { 0, 0 });
    g.setColour (juce::Colours::grey);
    g.drawLine (juce::Line<int> { originViewportPosition.getX(), originViewportPosition.getY(), speakerViewportPosition.getX(), speakerViewportPosition.getY() }.toFloat());
}

void SpeakerVisualizationContainer::timerCallback()
{
    repaint();
}
