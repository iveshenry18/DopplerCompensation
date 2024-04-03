//
// Created by Henry Ives on 4/3/24.
//

#include "DopplerSpinner.h"

void DopplerSpinner::prepareToPlay (double sampleRate)
{
    mSmoothedDiameter.reset (sampleRate, 0.1);
    mSmoothedDistanceToFocalPoint.reset (sampleRate, 0.1);
    mSmoothedPhaseOffset.reset (sampleRate, 0.1);
    mSmoothedSpinRate.reset (sampleRate, 0.1);
}

void DopplerSpinner::updateParams (float diameter, float distanceToFocalPoint, float spinRate, float phaseOffset)
{
    mSmoothedDiameter.setTargetValue (diameter);
    mSmoothedDistanceToFocalPoint.setTargetValue (distanceToFocalPoint);
    mSmoothedSpinRate.setTargetValue (spinRate);
    mSmoothedPhaseOffset.setTargetValue (phaseOffset);
}

juce::Point<float> DopplerSpinner::getNextSpeakerPosition (bool incrementPhase)
{
    auto spinRate = mSmoothedSpinRate.getNextValue();
    auto phaseOffset = mSmoothedPhaseOffset.getNextValue();
    float secondsPerSpin = 1.f / abs (spinRate);
    bool spinClockwise = spinRate >= 0;
    float finalPhase = mPhase + (phaseOffset / 100.f);
    float y = -sin (juce::MathConstants<float>::twoPi * finalPhase);
    float x = cos (juce::MathConstants<float>::twoPi * finalPhase);

    if (incrementPhase)
    {
        // TODO: implement stateful phase
        float phaseIncrement = 0; // Something!!
        mPhase = mPhase + phaseIncrement;
    }

    return { x, y };
}

float DopplerSpinner::getNextTimeDelta (bool incrementPhase)
{
    auto diameter = mSmoothedDiameter.getNextValue();
    auto distanceToFocalPoint = mSmoothedDistanceToFocalPoint.getNextValue();
    auto spinnerRadius = diameter / 2;

    // Compute Delay Time
    // (All positions in meters)
    juce::Point<float> focalPointPosition = { 0, -(distanceToFocalPoint) };
    juce::Point<float> phantomSpeakerPosition = { 0, spinnerRadius };

    float phantomSpeakerDist = phantomSpeakerPosition.getDistanceFrom (focalPointPosition);

    juce::Point<float> speakerPosition = getNextSpeakerPosition (incrementPhase);
    float realSpeakerDist = speakerPosition.getDistanceFrom (focalPointPosition);
    float distDelta = phantomSpeakerDist - realSpeakerDist;

    return distDelta / SPEED_OF_SOUND_MS;
}