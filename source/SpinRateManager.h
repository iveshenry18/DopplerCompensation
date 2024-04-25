//
// Created by Henry Ives on 4/19/24.
//

#pragma once

#include "SerialPort.h"
#include <juce_audio_processors/juce_audio_processors.h>

const std::pair<int, std::string> MANUAL_SOURCE = { 0, "Manual" };

class PluginProcessor;

// Possible: use a different program to take serial to OSC, then read the OSC
class SpinRateManager : public juce::HighResolutionTimer
{
public:
    SpinRateManager (PluginProcessor* pp);
    std::vector<std::pair<juce::String, bool>> getAvailableSources()
    {
        return mAvailableSources;
    }

    void updateParams();
    void prepareToPlay (double sampleRate);
    void processBlock (int numSamples);
    float getSpinRate()
    {
        int sign = invertSpinRate ? -1 : 1;
        return sign * mSmoothedSpinRate.getCurrentValue();
    }
    void hiResTimerCallback() override
    {
        readValues();
    }
    void setSource (int source);
    int getSource()
    {
        return mCurrentPort;
    }
    void setOnSourceChange (std::function<void()> fn)
    {
        onSourceChange = fn;
    }
    void setInvertSpinRate (bool invert)
    {
        invertSpinRate = invert;
    }

private:
    void updateAvailableSources();

    int mCurrentPort = MANUAL_SOURCE.first;
    bool invertSpinRate = false;
    PluginProcessor* mPluginProcessor;
    std::vector<std::pair<juce::String, bool>> mAvailableSources;
    std::unique_ptr<SerialPort> mSerialPort;
    juce::SmoothedValue<float> mSmoothedSpinRate;
    juce::AudioParameterFloat* mManualSpinRate;
    void readValues();
    std::function<void()> onSourceChange;
};
