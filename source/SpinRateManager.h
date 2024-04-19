//
// Created by Henry Ives on 4/19/24.
//

#pragma once

#include "SerialPort.h"
#include <juce_audio_processors/juce_audio_processors.h>

const std::pair<int, std::string> MANUAL_SOURCE = { 0, "Manual" };

class PluginProcessor;

// Possible: use a different program to take serial to OSC, then read the OSC
class SpinRateManager : public juce::AudioProcessorParameter::Listener, public juce::HighResolutionTimer
{
public:
    SpinRateManager (PluginProcessor* pp);
    juce::StringArray getAvailableSources()
    {
        return mAvailableSources;
    }
    void parameterValueChanged (int parameterIndex, float newValue) override;
    void parameterGestureChanged (int parameterIndex, bool gestureIsStarting) override {}

    void updateParams();
    void prepareToPlay (double sampleRate);
    void processBlock (int numSamples);
    float getSpinRate()
    {
        return mSmoothedSpinRate.getCurrentValue();
    }
    void hiResTimerCallback() override
    {
        readValues();
    }

private:
    void updateAvailableSources();

    PluginProcessor* mPluginProcessor;
    juce::StringArray mAvailableSources;
    std::unique_ptr<SerialPort> mSerialPort;
    juce::SmoothedValue<float> mSmoothedSpinRate;
    juce::AudioParameterFloat* mManualSpinRate;
    void readValues();
};
