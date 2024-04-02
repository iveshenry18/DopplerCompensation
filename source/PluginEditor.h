/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "PluginProcessor.h"
#include <juce_audio_processors/juce_audio_processors.h>

typedef juce::AudioProcessorValueTreeState::SliderAttachment SliderAttachment;

//==============================================================================
/**
*/
class PluginEditor : public juce::AudioProcessorEditor
{
public:
    PluginEditor (PluginProcessor&);
    ~PluginEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    PluginProcessor& audioProcessor;

    juce::Slider mDiameterSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mDiameterSliderAttachment;
    juce::Label mDiameterLabel;
    juce::Slider mDistanceToFocalPointSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mDistanceToFocalPointSliderAttachment;
    juce::Label mDistanceToFocalPointLabel;
    juce::Slider mSpinRateSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mSpinRateSliderAttachment;
    juce::Label mSpinRateLabel;
    juce::Slider mPhaseSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mPhaseSliderAttachment;
    juce::Label mPhaseLabel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginEditor)
};
