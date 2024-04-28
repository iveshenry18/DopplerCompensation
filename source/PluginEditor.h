/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "PluginProcessor.h"
#include "SpeakerVisualizationContainer.h"
#include "DelayTimeVisualizationContainer.h"
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

    void updateSpinRateSources();

    SpeakerVisualizationContainer speakerVisualizationContainer;
    DelayTimeVisualizationContainer delayTimeVisualizationContainer;

    juce::Slider mDiameterSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mDiameterSliderAttachment;
    juce::Label mDiameterLabel;
    juce::Slider mDistanceToFocalPointSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mDistanceToFocalPointSliderAttachment;
    juce::Label mDistanceToFocalPointLabel;
    juce::Slider mSpinRateSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mSpinRateSliderAttachment;
    juce::Label mSpinRateLabel;
    juce::Slider mPhaseOffsetSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mPhaseOffsetSliderAttachment;
    juce::Label mPhaseOffsetLabel;
    juce::ComboBox mSpinRateSourceSelector;
    juce::TextButton mRefreshSpinRateSourcesButton;
    juce::Label mSpinRateSourceSelectorLabel;
    juce::Label mCurrentSpinRateLabel;
    juce::Label mCurrentSpinRateValue;
    juce::ToggleButton mInvertSpinRateButton;
    juce::Label mInvertSpinRateLabel;
    juce::ToggleButton mTestModeButton;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> mTestModeButtonAttachment;
    juce::Label mTestModeLabel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginEditor)
};
