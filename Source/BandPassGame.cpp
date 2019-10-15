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
        // Add filter
        IIRFilter* filter = new IIRFilter();
        filters.add(filter);
    }

    // initialize summary
    for(auto freq: freqList) {
        auto summaryItem = new SummaryItem;
        summaryItem->freq = freq;
        summaryItem->totalAttempts = 0;
        summaryItem->totalWins = 0;
        summary.add(summaryItem);
    }
}

BandPassGame::~BandPassGame()
{
    for(IIRFilter* filter: filters) {
        delete filter;
    }

    // clean summary items
    for(auto item: summary) {
        delete item;
    }
}

Array<SummaryItem*> BandPassGame::getSummary() {
    return summary;
}

bool BandPassGame::isStarted() {
    return started;
}

void BandPassGame::start()
{
    int index = random.nextInt(freqList.size());
    currentFreq = freqList[index];

    for (IIRFilter* filter: filters) {
        filter->setCoefficients(IIRCoefficients::makeBandPass(sampleRate, currentFreq, 5.0f));
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

    // update the summary
    SummaryItem* item;
    for(auto i: summary) {
        if (i->freq == currentFreq) {
            item = i;
            break;
        }
    }

    item->totalAttempts += 1;
    item->totalWins += answeredFreq == currentFreq ? 1 : 0;
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

Array<float> BandPassGame::getFreqList() {
    return freqList;
}
