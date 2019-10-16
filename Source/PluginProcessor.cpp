/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
EQGameAudioProcessor::EQGameAudioProcessor()
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

EQGameAudioProcessor::~EQGameAudioProcessor()
{
}

//==============================================================================
const String EQGameAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool EQGameAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool EQGameAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool EQGameAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double EQGameAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int EQGameAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int EQGameAudioProcessor::getCurrentProgram()
{
    return 0;
}

void EQGameAudioProcessor::setCurrentProgram (int index)
{
}

const String EQGameAudioProcessor::getProgramName (int index)
{
    return {};
}

void EQGameAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void EQGameAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    bandPassGame.setSampleRate(sampleRate);
}

void EQGameAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool EQGameAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void EQGameAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
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
bool EQGameAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* EQGameAudioProcessor::createEditor()
{
    return new EQGameAudioProcessorEditor (*this);
}

String EQGameAudioProcessor::freqToString(float freq) {
    return "freq" + String((int) freq);
}

//==============================================================================
void EQGameAudioProcessor::getStateInformation (MemoryBlock& destData)
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

void EQGameAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
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
    return new EQGameAudioProcessor();
}
