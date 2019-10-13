/*
  ==============================================================================

    BandPassGame.cpp
    Created: 10 Oct 2019 3:31:08pm
    Author:  Arunoda Susiripala

  ==============================================================================
*/

#include "BandPassGame.h"

BandPassGame::BandPassGame(int numChannels)
{
    for(int lc=0; lc<numChannels; lc++) {
        IIRFilter* filter = new IIRFilter();
        filters.add(filter);
    }

}

BandPassGame::~BandPassGame()
{
    for(int lc=0; lc<filters.size(); lc++) {
        delete filters[lc];
    }
}

void BandPassGame::start()
{
    int index = random.nextInt(6);
    currentFreq = freqList[index];

    for(int lc=0; lc<filters.size(); lc++) {
        filters[lc]->setCoefficients(IIRCoefficients::makeBandPass(sampleRate, currentFreq, 5.0f));
    }
    started = true;
    answeredFreq = 0.0f;
}

void BandPassGame::setSampleRate(float s) {
    sampleRate = s;
    if (started) {
        start();
    }
}

void BandPassGame::setAnswer(float freq) {
    answeredFreq = freq;
    started = false;
}

void BandPassGame::apply(AudioBuffer<float>& buffer, int makeUpGain) {
    if (started) {
        for(int lc=0; lc<filters.size(); lc++) {
            float* channel = buffer.getWritePointer(lc);
            for(int ilc=0; ilc<buffer.getNumSamples(); ilc++) {
                channel[ilc] *= makeUpGain;
            }
            filters[lc]->processSamples(channel, buffer.getNumSamples());
        }
    }
}

float BandPassGame::getCurrentFreq() {
    return currentFreq;
}

float BandPassGame::getAnswer() {
    return answeredFreq;
}

float* BandPassGame::getFreqList() {
    return freqList;
}
