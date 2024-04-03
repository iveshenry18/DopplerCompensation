//
// Created by Henry Ives on 3/1/24.
//

#ifndef APICPPDELAY_DELAY_H
#define APICPPDELAY_DELAY_H

#include <juce_audio_processors/juce_audio_processors.h>

class Delay
{
public:
    Delay();

    ~Delay();

    void prepareToPlay (float inSampleRate);

    void processBlock (juce::AudioBuffer<float>& inBuffer);

    /**
     * @param inDelayTimeSeconds [0.01, 1]
     * @param inFeedbackPercent  [0, 1]
     * @param inDryWetPercent [0, 1]
     */
    void setParameters (float inDelayTimeSeconds, float inFeedbackPercent, float inDryWetPercent);

private:
    /* Circular Buffer */
    juce::AudioBuffer<float> mCircularBuffer;

    /* Playheads */
    int mWritehead = 0;
    float mFeedbackLeft = 0;
    float mFeedbackRight = 0;

    const float mMaxDelayTimeSeconds = 1.f;
    float mSampleRate = 44100.f;

    /* Smoothed Values */
    juce::LinearSmoothedValue<float> mSmoothedDryWet;
    juce::LinearSmoothedValue<float> mSmoothedFeedback;
    juce::LinearSmoothedValue<float> mSmoothedTime;
};

#endif //APICPPDELAY_DELAY_H
