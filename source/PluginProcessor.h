/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <juce_audio_processors/juce_audio_processors.h>

#include "Delay.h"
#include "SyncManager.h"
#include <utility>

const float SPEED_OF_SOUND_MS = 343;

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

private:
    float mSampleRate = 44100;
    // If the host doesn't provide transport info, we need to keep track of sample time ourselves
    int mSamplesPerBlock = 512;

    // Just so you know at 44.1kHz this will overflow if the program runs for 6.6 million years.
    // It also happens to be exactly how the playhead behaves in standalone mode
    int64_t mTimeInSamples = 0;

    juce::AudioParameterFloat* mDiameter = nullptr;
    juce::AudioParameterFloat* mDistanceToFocalPoint = nullptr;
    juce::AudioParameterFloat* mSpinRate = nullptr;
    juce::AudioParameterFloat* mPhase = nullptr;

    std::unique_ptr<juce::AudioProcessorValueTreeState> mValueTreeState;
    void _constructValueTreeStates();

    Delay delay;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginProcessor)
    juce::Point<float> computeSpeakerPosition (float seconds);
};
