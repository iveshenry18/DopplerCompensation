//
// Created by Henry Ives on 3/1/24.
//
#include "Delay.h"

Delay::Delay() = default;

Delay::~Delay() = default;

void Delay::prepareToPlay(float inSampleRate) {
    mSampleRate = inSampleRate;
    mCircularBuffer.setSize(2, mMaxDelayTimeSeconds * inSampleRate);
    mCircularBuffer.clear();

    mSmoothedDryWet.reset(inSampleRate, .01f);
    mSmoothedFeedback.reset(inSampleRate, .01f);
    mSmoothedTime.reset(inSampleRate, .01f);
}

void Delay::processBlock(juce::AudioBuffer<float>& inBuffer) {
    int num_samples = inBuffer.getNumSamples();

    float* in_channel_left = inBuffer.getWritePointer(0);
    float* in_channel_right = inBuffer.getWritePointer(1);

    float* circular_channel_left = mCircularBuffer.getWritePointer(0);
    float* circular_channel_right = mCircularBuffer.getWritePointer(1);

    for (int i = 0; i < num_samples; i++) {
        float input_left = in_channel_left[i];
        float input_right = in_channel_right[i];

        in_channel_left[i] = 0.f;
        in_channel_right[i] = 0.f;

        circular_channel_left[mWritehead] = input_left;
        circular_channel_right[mWritehead] = input_right;

        int delay_time_samples = static_cast<int> (mSmoothedTime.getNextValue() * mSampleRate);
        int readhead = mWritehead - delay_time_samples;

        if (readhead < 0) {
            readhead += mCircularBuffer.getNumSamples();
        }

        // pack the output
        float delay_out_left = circular_channel_left[readhead];
        float delay_out_right = circular_channel_right[readhead];

        float mix_amount = mSmoothedDryWet.getNextValue();

        float dry_amount = (1.f - mix_amount);
        float wet_amount = mix_amount;

        float feedback_amount = mSmoothedFeedback.getNextValue();
        mFeedbackLeft = delay_out_left * feedback_amount;
        mFeedbackRight = delay_out_right * feedback_amount;



        in_channel_left[i] = (input_left * dry_amount) + ((delay_out_left + mFeedbackLeft) * wet_amount);
        in_channel_right[i] = (input_right * dry_amount) + ((delay_out_right + mFeedbackRight) * wet_amount);


        ++mWritehead;
        if (mWritehead >= mCircularBuffer.getNumSamples()) {
            mWritehead = 0;
        }

    }
}

/**
 * @param inDelayTimeSeconds [0.01, 1]
 * @param inFeedbackPercent  [0, 1]
 * @param inDryWetPercent [0, 1]
 */
void Delay::setParameters(float inDelayTimeSeconds, float inFeedbackPercent, float inDryWetPercent) {
    mSmoothedTime.setTargetValue(inDelayTimeSeconds);
    mSmoothedFeedback.setTargetValue(inDryWetPercent);
    mSmoothedDryWet.setTargetValue(inDryWetPercent);
}

