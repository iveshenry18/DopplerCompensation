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
    : AudioProcessorEditor (&parent), audioProcessor (parent), speakerVisualizationContainer (audioProcessor.getDopplerSpinner())
{
    mDiameterSlider.setRange (0, 5);
    mDiameterSlider.setTextBoxStyle (juce::Slider::TextBoxBelow, true, 100, 20);
    mDiameterSlider.setTextBoxIsEditable (true);
    mDiameterSlider.setTitle ("Diameter (m)");
    mDiameterSlider.setHelpText ("This is the spinnerDiameter of the spinner in meters");
    mDiameterSlider.setSliderStyle (juce::Slider::SliderStyle::RotaryVerticalDrag);
    mDiameterSliderAttachment.reset (new juce::AudioProcessorValueTreeState::SliderAttachment (
        audioProcessor.getVTS(),
        "spinnerDiameter",
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

    mSpinRateSourceSelectorAttachment.reset (new juce::AudioProcessorValueTreeState::ComboBoxAttachment (
        audioProcessor.getVTS(),
        "spin_rate_source",
        mSpinRateSourceSelector));
    auto spinRateSourceParam = audioProcessor.getVTS().getParameter ("spin_rate_source");
    mSpinRateSourceSelector.addItemList (spinRateSourceParam->getAllValueStrings(), 1);
    // TODO: get init value correctly
//    mSpinRateSourceSelector.setItemEnabled (spinRateSourceParam->getValue(), true);

    mSpinRateSourceSelectorLabel.setText ("Spin Rate Source", juce::dontSendNotification);
    mSpinRateSourceSelectorLabel.setJustificationType (juce::Justification::centred);

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

    mTestModeButton.setClickingTogglesState (true);
    mTestModeButton.setTitle ("Test Mode");
    mTestModeButton.setHelpText ("Activates Test Mode");
    mTestModeButtonAttachment.reset (new juce::AudioProcessorValueTreeState::ButtonAttachment (
        audioProcessor.getVTS(),
        "test_mode",
        mTestModeButton));

    mTestModeLabel.setText ("Test Mode", juce::dontSendNotification);
    mTestModeLabel.setJustificationType (juce::Justification::right);

    addAndMakeVisible (mDiameterSlider);
    addAndMakeVisible (mDiameterLabel);
    addAndMakeVisible (mDistanceToFocalPointSlider);
    addAndMakeVisible (mDistanceToFocalPointLabel);
    addAndMakeVisible (mSpinRateSlider);
    addAndMakeVisible (mSpinRateLabel);
    addAndMakeVisible (mPhaseOffsetSlider);
    addAndMakeVisible (mPhaseOffsetLabel);
    addAndMakeVisible (mTestModeButton);
    addAndMakeVisible (mTestModeLabel);
    addAndMakeVisible (speakerVisualizationContainer);
    addAndMakeVisible (mSpinRateSourceSelector);
    addAndMakeVisible (mSpinRateSourceSelectorLabel);
    setSize (800, 300);
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
    auto visualizerArea = bounds.removeFromRight (int (getWidth() * .3));
    auto knobArea = bounds.removeFromTop (int (getHeight() * .6));
    auto labelArea = bounds.removeFromTop (int (getHeight() * .2));
    auto bottomArea = bounds.removeFromBottom (int (getHeight() * .2));

    speakerVisualizationContainer.setBounds (visualizerArea);

    mDiameterSlider.setBounds (knobArea.removeFromLeft (knobArea.getWidth() / 4));
    mDistanceToFocalPointSlider.setBounds (knobArea.removeFromLeft (knobArea.getWidth() / 3));
    mSpinRateSlider.setBounds (knobArea.removeFromLeft (knobArea.getWidth() / 2));
    mPhaseOffsetSlider.setBounds (knobArea.removeFromLeft (knobArea.getWidth()));

    mDiameterLabel.setBounds (labelArea.removeFromLeft (labelArea.getWidth() / 4));
    mDistanceToFocalPointLabel.setBounds (labelArea.removeFromLeft (labelArea.getWidth() / 3));
    mSpinRateLabel.setBounds (labelArea.removeFromLeft (labelArea.getWidth() / 2));
    mPhaseOffsetLabel.setBounds (labelArea.removeFromLeft (labelArea.getWidth()));

    mTestModeLabel.setBounds (bottomArea.removeFromLeft (bottomArea.getWidth() / 4));
    mTestModeButton.setBounds (bottomArea.removeFromLeft (bottomArea.getWidth() / 3));
    mSpinRateSourceSelectorLabel.setBounds (bottomArea.removeFromLeft (bottomArea.getWidth() / 2));
    mSpinRateSourceSelector.setBounds (bottomArea.removeFromLeft (bottomArea.getWidth()));
}
