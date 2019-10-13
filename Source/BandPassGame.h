/*
  ==============================================================================

    BandPassGame.h
    Created: 10 Oct 2019 3:31:08pm
    Author:  Arunoda Susiripala

  ==============================================================================
*/

#pragma once

#ifndef GOLDENEARSEQ_BANDPASSGAME__H
#define GOLDENEARSEQ_BANDPASSGAME__H

#include "../JuceLibraryCode/JuceHeader.h"

class BandPassGame {
public:
    BandPassGame(int numChannels);
    ~BandPassGame();
    // For Editor
    float* getFreqList();
    float getAnswer();
    float getCurrentFreq();
    void start();
    bool isStarted();
    void setAnswer(float freq);

    // For Processor
    void setSampleRate(float sampleRate);
    void apply(AudioBuffer<float>& buffer, int makeUpGain);

private:
    Array<IIRFilter*> filters;
    bool started = false;
    float answeredFreq = 0.0f;
    float currentFreq = 0.0f;
    float freqList[7] = {125.0f, 250.0f, 500.0f, 1000.0f, 2000.0f, 4000.0f, 8000.0f};
    Random random;
    float sampleRate;
};


#endif //GOLDENEARSEQ_BANDPASSGAME__H

