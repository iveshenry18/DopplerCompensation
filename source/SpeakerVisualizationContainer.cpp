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
    SpinnerState spinnerState = mDopplerSpinner->getCurrentState();

    g.fillAll (juce::Colours::whitesmoke);
    if (juce::approximatelyEqual (spinnerState.spinnerDiameter, 0.f) && juce::approximatelyEqual (spinnerState.distanceToFocalPoint, 0.f))
    {
        DBG("No diameter or distanceToFocalPoint");
        mErrorMessage.setText ("Spinner parameters not initialized. Is your audio running?", juce::dontSendNotification);
        mErrorMessage.setJustificationType (juce::Justification::centred);
        mErrorMessage.centreWithSize (getWidth() / 3, getHeight() / 5);
        addAndMakeVisible (mErrorMessage);
    }
    else
    {
        auto physicalBounds = juce::Rectangle<float> {}.withCentre ({ 0, 0 }).withSizeKeepingCentre (spinnerState.spinnerDiameter * 1.1, std::max (spinnerState.spinnerDiameter / 2, spinnerState.distanceToFocalPoint) * 2.4);
        mPhysicalToViewport.setPhysicalBounds (physicalBounds);
        mPhysicalToViewport.setViewportBounds (getBounds());

        drawGridlines (g);

        mPhantomSpeaker = juce::Rectangle<float>().withCentre (spinnerState.phantomSpeakerPosition).withSizeKeepingCentre (0.08, 0.08);
        g.setColour (juce::Colours::lightgrey);
        g.fillEllipse (mPhysicalToViewport.transform (mPhantomSpeaker).toFloat());

        mCircumference = juce::Rectangle<float> { 0, 0 }.withCentre ({ 0, 0 }).withSizeKeepingCentre (spinnerState.spinnerDiameter, spinnerState.spinnerDiameter);
        g.setColour (juce::Colours::lightgrey);
        g.drawEllipse (mPhysicalToViewport.transform (mCircumference).toFloat(), 1);

        mSpeakerPhysical = juce::Rectangle<float>().withCentre (spinnerState.speakerPosition).withSizeKeepingCentre (0.08, 0.08);
        g.setColour (juce::Colours::black);
        mSpeakerViewport = mPhysicalToViewport.transform (mSpeakerPhysical).toFloat();
        g.fillEllipse (mSpeakerViewport);

        mFocalPointBody = juce::Rectangle<float>().withCentre ({ 0, -spinnerState.distanceToFocalPoint }).withSizeKeepingCentre (0.41, 0.2);
        g.setColour (juce::Colours::blue);
        g.fillEllipse (mPhysicalToViewport.transform (mFocalPointBody).toFloat());
        mFocalPointHead = juce::Rectangle<float>().withCentre ({ 0, -spinnerState.distanceToFocalPoint }).withSizeKeepingCentre (0.19, 0.2);
        g.setColour (juce::Colours::lightsalmon);
        g.fillEllipse (mPhysicalToViewport.transform (mFocalPointHead).toFloat());

        juce::Point<int> originViewportPosition = mPhysicalToViewport.transform (juce::Point<float> { 0, 0 });
        g.setColour (juce::Colours::grey);
        g.drawLine (juce::Line<int> { originViewportPosition.getX(), originViewportPosition.getY(), (int) mSpeakerViewport.getCentreX(), (int) mSpeakerViewport.getCentreY() }.toFloat());
    }
}

void SpeakerVisualizationContainer::timerCallback()
{
    repaint();
}
