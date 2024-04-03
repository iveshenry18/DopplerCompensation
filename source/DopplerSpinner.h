//
// Created by Henry Ives on 4/3/24.
//

#pragma once

#include <juce_audio_processors/juce_audio_processors.h>

const float SPEED_OF_SOUND_MS = 343;

class DopplerSpinner
{
public:
    void prepareToPlay (double sampleRate);
    void updateParams (float diameter, float distanceToFocalPoint, float spinRate, float phaseOffset);
    float getNextTimeDelta(bool incrementPhase = true);

private:
    juce::SmoothedValue<float> mSmoothedDiameter = 0;
    juce::SmoothedValue<float> mSmoothedDistanceToFocalPoint = 0;
    juce::SmoothedValue<float> mSmoothedSpinRate = 0;
    juce::SmoothedValue<float> mSmoothedPhaseOffset = 0;

    juce::Point<float> getNextSpeakerPosition (bool incrementPhase = true);
    float mPhase = 0;
};
