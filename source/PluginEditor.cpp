/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginEditor.h"

#include "PluginProcessor.h"
#include <memory>

//==============================================================================
PluginEditor::PluginEditor (PluginProcessor& parent)
    : AudioProcessorEditor (&parent), audioProcessor (parent)
{
    mDiameterSlider.setRange (0, 5);
    mDiameterSlider.setTextBoxStyle (juce::Slider::TextBoxBelow, true, 100, 20);
    mDiameterSlider.setTextBoxIsEditable (true);
    mDiameterSlider.setTitle ("Diameter (m)");
    mDiameterSlider.setHelpText ("This is the diameter of the spinner in meters");
    mDiameterSlider.setSliderStyle (juce::Slider::SliderStyle::RotaryVerticalDrag);
    mDiameterSliderAttachment.reset (new juce::AudioProcessorValueTreeState::SliderAttachment (
        audioProcessor.getVTS(),
        "diameter",
        mDiameterSlider));
    mDiameterLabel.setText ("Diameter", juce::dontSendNotification);
    mDiameterLabel.setJustificationType (juce::Justification::centred);

    mDistanceToFocalPointSlider.setRange (0, 10);
    mDistanceToFocalPointSlider.setTextBoxStyle (juce::Slider::TextBoxBelow, true, 100, 20);
    mDistanceToFocalPointSlider.setTextBoxIsEditable (true);
    mDistanceToFocalPointSlider.setTitle ("Distance to Focal Point");
    mDistanceToFocalPointSlider.setHelpText ("This is the distance to the focal point in meters");
    mDistanceToFocalPointSlider.setSliderStyle (juce::Slider::SliderStyle::RotaryVerticalDrag);
    mDistanceToFocalPointSliderAttachment.reset (new juce::AudioProcessorValueTreeState::SliderAttachment (
        audioProcessor.getVTS(),
        "distance_to_focal_point",
        mDistanceToFocalPointSlider));
    mDistanceToFocalPointLabel.setText ("Distance to Focal Point (m)", juce::dontSendNotification);
    mDistanceToFocalPointLabel.setJustificationType (juce::Justification::centred);

    mSpinRateSlider.setRange (0, 10);
    mSpinRateSlider.setTextBoxStyle (juce::Slider::TextBoxBelow, true, 100, 20);
    mSpinRateSlider.setTextBoxIsEditable (true);
    mSpinRateSlider.setTitle ("Spin Rate");
    mSpinRateSlider.setHelpText ("This is the rate of the spinner in rotations per second");
    mSpinRateSlider.setSliderStyle (juce::Slider::SliderStyle::RotaryVerticalDrag);
    mSpinRateSliderAttachment.reset (new juce::AudioProcessorValueTreeState::SliderAttachment (
        audioProcessor.getVTS(),
        "spin_rate",
        mSpinRateSlider));

    mSpinRateLabel.setText ("Spin Rate (rps)", juce::dontSendNotification);
    mSpinRateLabel.setJustificationType (juce::Justification::centred);

    mPhaseOffsetSlider.setRange (0, 100);
    mPhaseOffsetSlider.setTextBoxStyle (juce::Slider::TextBoxBelow, true, 100, 20);
    mPhaseOffsetSlider.setTextBoxIsEditable (true);
    mPhaseOffsetSlider.setTitle ("Phase (%)");
    mPhaseOffsetSlider.setHelpText ("This is the Phase of the spinner. Use it to rotate the focal point.");
    mPhaseOffsetSlider.setSliderStyle (juce::Slider::SliderStyle::RotaryVerticalDrag);
    mPhaseOffsetSliderAttachment.reset (new juce::AudioProcessorValueTreeState::SliderAttachment (
        audioProcessor.getVTS(),
        "phase_offset",
        mPhaseOffsetSlider));

    mPhaseOffsetLabel.setText ("Phase Offset", juce::dontSendNotification);
    mPhaseOffsetLabel.setJustificationType (juce::Justification::centred);

    addAndMakeVisible (mDiameterSlider);
    addAndMakeVisible (mDiameterLabel);
    addAndMakeVisible (mDistanceToFocalPointSlider);
    addAndMakeVisible (mDistanceToFocalPointLabel);
    addAndMakeVisible (mSpinRateSlider);
    addAndMakeVisible (mSpinRateLabel);
    addAndMakeVisible (mPhaseOffsetSlider);
    addAndMakeVisible (mPhaseOffsetLabel);

    setSize (500, 300);
}

PluginEditor::~PluginEditor() = default;

//==============================================================================
void PluginEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (juce::Colours::grey);
}

void PluginEditor::resized()
{
    auto bounds = getLocalBounds();
    auto knob_area = bounds.removeFromTop (int (getHeight() * .7));
    auto label_area = bounds.removeFromTop (int (getHeight() * .3));

    mDiameterSlider.setBounds (knob_area.removeFromLeft (getWidth() / 4));
    mDistanceToFocalPointSlider.setBounds (knob_area.removeFromLeft (getWidth() / 4));
    mSpinRateSlider.setBounds (knob_area.removeFromLeft (getWidth() / 4));
    mPhaseOffsetSlider.setBounds (knob_area.removeFromLeft (getWidth() / 4));

    mDiameterLabel.setBounds (label_area.removeFromLeft (getWidth() / 4));
    mDistanceToFocalPointLabel.setBounds (label_area.removeFromLeft (getWidth() / 4));
    mSpinRateLabel.setBounds (label_area.removeFromLeft (getWidth() / 4));
    mPhaseOffsetLabel.setBounds (label_area.removeFromLeft (getWidth() / 4));
}
