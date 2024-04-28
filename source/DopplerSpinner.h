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

class DopplerSpinner: public juce::HighResolutionTimer
{
public:
    void initTimer (double updateRateHz);
    void updateParams (float diameter, float distanceToFocalPoint, float spinRate, float phaseOffset);
    void hiResTimerCallback() override;
    SpinnerState getCurrentState();

private:
    juce::SmoothedValue<float> mSmoothedDiameter = 0;
    juce::SmoothedValue<float> mSmoothedDistanceToFocalPoint = 0;
    juce::SmoothedValue<float> mSmoothedSpinRate = 0;
    juce::SmoothedValue<float> mSmoothedPhaseOffset = 0;

    SpinnerState mCurrentState;

    juce::Point<float> getNextSpeakerPosition ();
    float mPhase = 0;
};
