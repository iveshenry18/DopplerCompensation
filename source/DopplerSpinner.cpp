//
// Created by Henry Ives on 4/3/24.
//

#include "DopplerSpinner.h"
#include "PluginProcessor.h"

void DopplerSpinner::prepareToPlay (double sampleRate)
{
    // To avoid noise in the delay time, we filter out any frequencies 10x above the max spin rate.
    iir.reset();
    iir.setCoefficients(juce::IIRCoefficients::makeLowPass(sampleRate, MAX_SPIN_RATE * 10));

    mUpdateRateHz = sampleRate;
    mSmoothedDiameter.reset (sampleRate, 0.1);
    mSmoothedDistanceToFocalPoint.reset (sampleRate, 0.1);
    mSmoothedPhaseOffset.reset (sampleRate, 0.1);
    mSmoothedSpinRate.reset (sampleRate, 0.1);
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

    float phaseIncrement = spinRate / mUpdateRateHz;
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
void DopplerSpinner::updateState()
{
    float diameter = mSmoothedDiameter.getNextValue();
    float spinnerRadius = diameter / 2;
    float distanceToFocalPoint = mSmoothedDistanceToFocalPoint.getNextValue();

    // Compute Delay Time
    // (All positions in meters)
    juce::Point<float> focalPointPosition = { 0, -(distanceToFocalPoint) };
    juce::Point<float> phantomSpeakerPosition = { 0, spinnerRadius };

    float phantomSpeakerDist = phantomSpeakerPosition.getDistanceFrom (focalPointPosition);

    juce::Point<float> speakerPosition = getNextSpeakerPosition();
    float realSpeakerDist = speakerPosition.getDistanceFrom (focalPointPosition);
    float distDelta = phantomSpeakerDist - realSpeakerDist;
    float timeDelta = distDelta / SPEED_OF_SOUND_MS;
    timeDelta = iir.processSingleSampleRaw(timeDelta);

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