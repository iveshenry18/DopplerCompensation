/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "Fraction.h"
#include "PluginEditor.h"

//==============================================================================
PluginProcessor::PluginProcessor() :
#ifndef JucePlugin_PreferredChannelConfigurations
                                     AudioProcessor (BusesProperties()
    #if !JucePlugin_IsMidiEffect
        #if !JucePlugin_IsSynth
                                                         .withInput ("Input", juce::AudioChannelSet::stereo(), true)
        #endif
                                                         .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
    #endif
                                     )
#endif
{
    _constructValueTreeStates();

    mDiameter = dynamic_cast<juce::AudioParameterFloat*> (mValueTreeState->getParameter ("diameter"));
    jassert (mDiameter != nullptr);
    mDistanceToFocalPoint = dynamic_cast<juce::AudioParameterFloat*> (mValueTreeState->getParameter ("distance_to_focal_point"));
    jassert (mDistanceToFocalPoint != nullptr);
    mSpinRate = dynamic_cast<juce::AudioParameterFloat*> (mValueTreeState->getParameter ("diameter"));
    jassert (mSpinRate != nullptr);
    mDiameter = dynamic_cast<juce::AudioParameterFloat*> (mValueTreeState->getParameter ("diameter"));
    jassert (mDiameter != nullptr);
}

PluginProcessor::~PluginProcessor() = default;

//==============================================================================
void PluginProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    mSampleRate = static_cast<float> (sampleRate);
    mSamplesPerBlock = samplesPerBlock;
    mTimeInSamples = 0;
    delay->prepareToPlay (static_cast<float> (sampleRate));
}

void PluginProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    int64_t bufferStartTimeSamples = getPlayHead()->getPosition()->getTimeInSamples().orFallback (mTimeInSamples);
    auto timeInSeconds = static_cast<float> (mTimeInSamples / bufferStartTimeSamples);
    auto spinnerRadius = mDiameter->get() / 2;

    // Compute Delay Time
    // (All positions in meters)
    juce::Point<float> focalPointPosition = { 0, -(mDistanceToFocalPoint->get()) };
    juce::Point<float> phantomSpeakerPosition = { 0, spinnerRadius };

    // float phantomSpeakerDistFull = sqrt(pow(0 - focalPointXPos, 2) + pow(spinnerRadius - focalPointYPos, 2))
    // which simplifies to:
    float phantomSpeakerDist = phantomSpeakerPosition.getDistanceFrom (focalPointPosition);

    juce::Point<float> speakerPosition = computeSpeakerPosition (timeInSeconds);
    float realSpeakerDist = speakerPosition.getDistanceFrom (focalPointPosition);
    float distDelta = phantomSpeakerDist - realSpeakerDist;

    // Adjust Delay Time
    float timeDelta = distDelta / SPEED_OF_SOUND_MS;

    // Run Delay
    delay->setParameters (timeDelta, 0, 1);
    delay->processBlock (buffer);

    // Update current time (if host doesn't provide time)
    mTimeInSamples += mSamplesPerBlock;
}

//==============================================================================
const juce::String PluginProcessor::getName() const
{
    return JucePlugin_Name;
}

bool PluginProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool PluginProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool PluginProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

double PluginProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int PluginProcessor::getNumPrograms()
{
    return 1; // NB: some hosts don't cope very well if you tell them there are 0 programs,
        // so this should be at least 1, even if you're not really implementing programs.
}

int PluginProcessor::getCurrentProgram()
{
    return 0;
}

void PluginProcessor::setCurrentProgram (int index)
{
}

const juce::String PluginProcessor::getProgramName (int index)
{
    return {};
}

void PluginProcessor::changeProgramName (int index, const juce::String& newName)
{
}

void PluginProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool PluginProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
    #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
        && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

            // This checks if the input layout matches the output layout
        #if !JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
        #endif

    return true;
    #endif
}
#endif

//==============================================================================
bool PluginProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* PluginProcessor::createEditor()
{
    auto editor = new PluginEditor (*this);
    return editor;
}

//==============================================================================
void PluginProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    auto state = mValueTreeState->copyState();
    std::unique_ptr<juce::XmlElement> xml (state.createXml());
    copyXmlToBinary (*xml, destData);
}

void PluginProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    std::unique_ptr<juce::XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));

    if (xmlState != nullptr)
        if (xmlState->hasTagName (mValueTreeState->state.getType()))
            mValueTreeState->replaceState (juce::ValueTree::fromXml (*xmlState));
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new PluginProcessor();
}
void PluginProcessor::_constructValueTreeStates()
{
    mValueTreeState.reset (new juce::AudioProcessorValueTreeState (*this, nullptr, juce::Identifier ("DopplerCompensationParams"),

        { std::make_unique<juce::AudioParameterFloat> (juce::ParameterID ("diameter", 1), // parameterID
              "Diameter (m)", // parameter name
              juce::NormalisableRange<float> (0, 5),
              1,
              juce::AudioParameterFloatAttributes().withStringFromValueFunction ([] (auto v1, auto v2) {
                  return std::to_string (v1) + " m";
              })),
            std::make_unique<juce::AudioParameterFloat> (juce::ParameterID ("distance_to_focal_point", 1), // parameterID
                "Distance to Focal Point (m)", // parameter name
                juce::NormalisableRange<float> (0, 10),
                2,
                juce::AudioParameterFloatAttributes().withStringFromValueFunction ([] (auto v1, auto v2) {
                    return std::to_string (v1) + " m";
                })),
            std::make_unique<juce::AudioParameterFloat> (juce::ParameterID ("spin_rate", 1), // parameterID
                "Spin Rate (rps)", // parameter name
                juce::NormalisableRange<float> (0, 10),
                1,
                juce::AudioParameterFloatAttributes().withStringFromValueFunction ([] (auto v1, auto v2) {
                    return std::to_string (v1) + " rps";
                })), // default value
            std::make_unique<juce::AudioParameterFloat> (juce::ParameterID ("phase", 1), // parameterID
                "Phase (%)", // parameter name
                juce::NormalisableRange<float> (0, 100),
                0,
                juce::AudioParameterFloatAttributes().withStringFromValueFunction ([] (auto v1, auto v2) {
                    return std::to_string (v1) + " %";
                })) }));
}
juce::Point<float> PluginProcessor::computeSpeakerPosition (float timeInSeconds)
{
    float secondsPerSpin = 1.f / abs (mSpinRate->get());
    bool spinClockwise = mSpinRate->get() >= 0;
    float phaseDecimal = ((timeInSeconds / secondsPerSpin) - trunc (timeInSeconds / secondsPerSpin) + (mPhase->get() / 100.f));
    phaseDecimal = spinClockwise ? phaseDecimal : -phaseDecimal;
    float y = -sin (juce::MathConstants<float>::twoPi * phaseDecimal);
    float x = cos (juce::MathConstants<float>::twoPi * phaseDecimal);
    return { x, y };
}
