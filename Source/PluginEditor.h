/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

//==============================================================================
/**
*/

class EQGameAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    EQGameAudioProcessorEditor (EQGameAudioProcessor&);
    ~EQGameAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    EQGameAudioProcessor& processor;

    TextButton btnStart;
    Label lblResult;
    Label lblFreqListHeader;
    Label lblSummary;
    Array<TextButton*> btnFreqList;
    Label lblRepoLink;

    HyperlinkButton btn;
    float getWinningPct();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EQGameAudioProcessorEditor)
};
