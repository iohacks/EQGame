/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
GoldenEarsEqAudioProcessor::GoldenEarsEqAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    addParameter(makeUpGain = new AudioParameterFloat("makeUpGain", "Make Up Gain", 1.0f, 10.0f, 2.0f));
}

GoldenEarsEqAudioProcessor::~GoldenEarsEqAudioProcessor()
{
}

//==============================================================================
const String GoldenEarsEqAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool GoldenEarsEqAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool GoldenEarsEqAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool GoldenEarsEqAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double GoldenEarsEqAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int GoldenEarsEqAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int GoldenEarsEqAudioProcessor::getCurrentProgram()
{
    return 0;
}

void GoldenEarsEqAudioProcessor::setCurrentProgram (int index)
{
}

const String GoldenEarsEqAudioProcessor::getProgramName (int index)
{
    return {};
}

void GoldenEarsEqAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void GoldenEarsEqAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    bandPassGame.setSampleRate(sampleRate);
}

void GoldenEarsEqAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool GoldenEarsEqAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void GoldenEarsEqAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    bandPassGame.apply(buffer, makeUpGain->get());
}

//==============================================================================
bool GoldenEarsEqAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* GoldenEarsEqAudioProcessor::createEditor()
{
    return new GoldenEarsEqAudioProcessorEditor (*this);
}

String GoldenEarsEqAudioProcessor::freqToString(float freq) {
    return "freq" + String((int) freq);
}

//==============================================================================
void GoldenEarsEqAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    XmlElement xmlSummary ("summary");
    for (auto item: bandPassGame.getSummary()) {
        auto xmlItem = new XmlElement(freqToString(item->freq));
        xmlItem->setAttribute("freq", item->freq);
        xmlItem->setAttribute("totalAttempts", item->totalAttempts);
        xmlItem->setAttribute("totalWins", item->totalWins);

        xmlSummary.addChildElement(xmlItem);
    }

    copyXmlToBinary(xmlSummary, destData);
}

void GoldenEarsEqAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    auto xmlSummary = getXmlFromBinary(data, sizeInBytes);
    if (xmlSummary != nullptr && xmlSummary->hasTagName("summary")) {
        for (auto item: bandPassGame.getSummary()) {
            auto xmlItem = xmlSummary->getChildByName(freqToString(item->freq));
            if (xmlItem) {
                item->totalWins = xmlItem->getDoubleAttribute("totalWins", 0.0f);
                item->totalAttempts = xmlItem->getDoubleAttribute("totalAttempts", 0.0f);
            } else {
                DBG("No XML item found for freq: " + String(item->freq));
            }
        }
    }
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new GoldenEarsEqAudioProcessor();
}
