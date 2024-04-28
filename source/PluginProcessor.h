/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_dsp/juce_dsp.h>

#include "DopplerSpinner.h"
#include "SpinRateManager.h"
#include "SyncManager.h"
#include <utility>

const int MAX_DELAY_IN_SAMPLES = 4096;
const float MAX_SPINNER_DIAMETER = 3.f;
const float MAX_DISTANCE_TO_FOCAL_POINT = 5.f;
const float MAX_SPIN_RATE = 5.f;

//==============================================================================
/**
*/
class PluginProcessor : public juce::AudioProcessor
{
public:
    //==============================================================================
    PluginProcessor();
    ~PluginProcessor() override;

    // AUDIO FUNCTIONS
    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    // PRESET / STATE FUNCTIONS
    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    //==============================================================================
    void releaseResources() override;

#ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
#endif

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;
    // EXPOSE VTS TO EDITOR
    juce::AudioProcessorValueTreeState& getVTS()
    {
        return *mValueTreeState;
    }

    SpinRateManager* getSpinRateManager()
    {
        return &mSpinRateManager;
    }

    DopplerSpinner* getDopplerSpinner()
    {
        return &mDopplerSpinner;
    }

private:
    float mSampleRate = 44100;
    // If the host doesn't provide transport info, we need to keep track of sample time ourselves
    int mSamplesPerBlock = 512;

    juce::AudioParameterFloat* mDiameter = nullptr;
    juce::AudioParameterFloat* mDistanceToFocalPoint = nullptr;
    juce::AudioParameterFloat* mPhaseOffset = nullptr;
    juce::AudioParameterBool* mTestMode = nullptr;
    juce::SmoothedValue<float> mSmoothedTimeDelta = 0;

    SpinRateManager mSpinRateManager;

    std::unique_ptr<juce::AudioProcessorValueTreeState> mValueTreeState;
    void _constructValueTreeStates();

    DopplerSpinner mDopplerSpinner;
    juce::dsp::DelayLine<float, juce::dsp::DelayLineInterpolationTypes::Lagrange3rd> delayLine = juce::dsp::DelayLine<float, juce::dsp::DelayLineInterpolationTypes::Lagrange3rd> (MAX_DELAY_IN_SAMPLES);

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginProcessor)
};
