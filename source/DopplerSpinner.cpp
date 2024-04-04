//
// Created by Henry Ives on 4/3/24.
//

#include "DopplerSpinner.h"

void DopplerSpinner::prepareToPlay (double sampleRate)
{
    mSampleRate = sampleRate;
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
    auto diameter = mSmoothedDiameter.getCurrentValue();
    auto spinnerRadius = diameter / 2;
    auto spinRate = mSmoothedSpinRate.getNextValue();
    auto phaseOffset = mSmoothedPhaseOffset.getNextValue();
    float finalPhase = mPhase + (phaseOffset / 100.f);
    float y = spinnerRadius * -sin (juce::MathConstants<float>::twoPi * finalPhase);
    float x = spinnerRadius * cos (juce::MathConstants<float>::twoPi * finalPhase);

    if (incrementPhase)
    {
        auto phaseIncrement = static_cast<float> (spinRate / mSampleRate);
        mPhase = mPhase + phaseIncrement;
    }

    return { x, y };
}

SpinnerState DopplerSpinner::getNextState (bool incrementPhase)
{
    auto diameter = mSmoothedDiameter.getNextValue();
    auto spinnerRadius = diameter / 2;
    auto distanceToFocalPoint = mSmoothedDistanceToFocalPoint.getNextValue();

    // Compute Delay Time
    // (All positions in meters)
    juce::Point<float> focalPointPosition = { 0, -(distanceToFocalPoint) };
    juce::Point<float> phantomSpeakerPosition = { 0, spinnerRadius };

    float phantomSpeakerDist = phantomSpeakerPosition.getDistanceFrom (focalPointPosition);

    juce::Point<float> speakerPosition = getNextSpeakerPosition (incrementPhase);
    float realSpeakerDist = speakerPosition.getDistanceFrom (focalPointPosition);
    float distDelta = phantomSpeakerDist - realSpeakerDist;
    float timeDelta = distDelta / SPEED_OF_SOUND_MS;

    auto gainFactor = static_cast<float> (pow (realSpeakerDist, 2) / pow (phantomSpeakerDist, 2));
    return SpinnerState {
        timeDelta,
        gainFactor,
        speakerPosition,
        phantomSpeakerPosition
    };
}