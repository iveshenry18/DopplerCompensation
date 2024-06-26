//
// Created by Henry Ives on 4/3/24.
//

#pragma once

#include <juce_audio_processors/juce_audio_processors.h>

const float SPEED_OF_SOUND_MS = 343;
struct SpinnerState
{
    float timeDeltaSeconds;
    float gainFactor;
    juce::Point<float> speakerPosition;
    juce::Point<float> phantomSpeakerPosition;
    bool isClicking;
    float spinnerDiameter;
    float distanceToFocalPoint;
};

class DopplerSpinner
{
public:
    void prepareToPlay (double sampleRate);
    void updateParams (float diameter, float distanceToFocalPoint, float spinRate, float phaseOffset);
    void updateState();
    SpinnerState getCurrentState();

private:
    juce::SmoothedValue<float> mSmoothedDiameter = 1;
    juce::SmoothedValue<float> mSmoothedDistanceToFocalPoint = 1;
    juce::SmoothedValue<float> mSmoothedSpinRate = 1;
    juce::SmoothedValue<float> mSmoothedPhaseOffset = 0;

    juce::IIRFilter iir;
    SpinnerState mCurrentState;

    juce::Point<float> getNextSpeakerPosition();
    float mPhase = 0;
    double mUpdateRateHz = 1;
};
