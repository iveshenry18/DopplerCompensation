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
    void updateParams (float diameter, float distanceToFocalPoint, float spinRate, float phaseOffset, double sampleRate);
    /**
     * Gets the next state of the spinner
     * @param incrementPhase when true, smoothed values and the spinner's position is updated.
     */
    SpinnerState getNextState (bool incrementPhase = true);

private:
    juce::SmoothedValue<float> mSmoothedDiameter = 0;
    juce::SmoothedValue<float> mSmoothedDistanceToFocalPoint = 0;
    juce::SmoothedValue<float> mSmoothedSpinRate = 0;
    juce::SmoothedValue<float> mSmoothedPhaseOffset = 0;
    double mSampleRate = 44100;

    juce::Point<float> getNextSpeakerPosition (bool incrementPhase = true);
    float mPhase = 0;
};
