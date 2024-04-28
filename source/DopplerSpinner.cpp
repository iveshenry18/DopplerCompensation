//
// Created by Henry Ives on 4/3/24.
//

#include "DopplerSpinner.h"

void DopplerSpinner::init (double updateRateHz)
{
    float intervalMs = 1000.f / updateRateHz;
    jassert(juce::approximatelyEqual(round(intervalMs), intervalMs));
    startTimer(round(intervalMs));
    mSmoothedDiameter.reset (updateRateHz, 0.1);
    mSmoothedDistanceToFocalPoint.reset (updateRateHz, 0.1);
    mSmoothedPhaseOffset.reset (updateRateHz, 0.1);
    mSmoothedSpinRate.reset (updateRateHz, 0.1);
}

// Consider thread-safety here
void DopplerSpinner::updateParams (float diameter, float distanceToFocalPoint, float spinRate, float phaseOffset)
{
    mSmoothedDiameter.setTargetValue (diameter);
    mSmoothedDistanceToFocalPoint.setTargetValue (distanceToFocalPoint);
    mSmoothedSpinRate.setTargetValue (spinRate);
    mSmoothedPhaseOffset.setTargetValue (phaseOffset);
}

/**
 * Assumed to be called once per update—otherwise the smoothed parameters will not
 * be changed at the correct rate.
 */
juce::Point<float> DopplerSpinner::getNextSpeakerPosition()
{
    auto diameter = mSmoothedDiameter.getCurrentValue();
    auto spinnerRadius = diameter / 2;
    auto spinRate = mSmoothedSpinRate.getNextValue();
    auto phaseOffset = mSmoothedPhaseOffset.getNextValue();
    float finalPhase = mPhase + (phaseOffset / 100.f);
    float y = spinnerRadius * -sin (juce::MathConstants<float>::twoPi * finalPhase);
    float x = spinnerRadius * cos (juce::MathConstants<float>::twoPi * finalPhase);

    float updateRateHz = 1000.f / getTimerInterval();
    float phaseIncrement = spinRate / updateRateHz;
    mPhase = mPhase + phaseIncrement;
    if (mPhase > 1)
    {
        mPhase -= 1;
    }

    return { x, y };
}

SpinnerState DopplerSpinner::getCurrentState()
{
    return mCurrentState;
}

/**
 * Update spinner state
 */
void DopplerSpinner::hiResTimerCallback()
{
    auto diameter = mSmoothedDiameter.getNextValue();
    auto spinnerRadius = diameter / 2;
    auto distanceToFocalPoint = mSmoothedDistanceToFocalPoint.getNextValue();

    // Compute Delay Time
    // (All positions in meters)
    juce::Point<float> focalPointPosition = { 0, -(distanceToFocalPoint) };
    juce::Point<float> phantomSpeakerPosition = { 0, spinnerRadius };

    float phantomSpeakerDist = phantomSpeakerPosition.getDistanceFrom (focalPointPosition);

    juce::Point<float> speakerPosition = getNextSpeakerPosition();
    float realSpeakerDist = speakerPosition.getDistanceFrom (focalPointPosition);
    float distDelta = phantomSpeakerDist - realSpeakerDist;
    float timeDelta = distDelta / SPEED_OF_SOUND_MS;

    auto gainFactor = static_cast<float> (pow (realSpeakerDist, 2) / pow (phantomSpeakerDist, 2));

    bool isClicking = phantomSpeakerPosition.getDistanceFrom (speakerPosition) < diameter / 200.f;
    mCurrentState = SpinnerState {
        timeDelta,
        gainFactor,
        speakerPosition,
        phantomSpeakerPosition,
        isClicking,
        diameter,
        distanceToFocalPoint
    };
}