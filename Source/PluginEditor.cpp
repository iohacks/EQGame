/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
GoldenEarsEqAudioProcessorEditor::GoldenEarsEqAudioProcessorEditor(GoldenEarsEqAudioProcessor &p)
        : AudioProcessorEditor(&p), processor(p) {
    // Add the freqList header
    lblFreqListHeader.setText("Select the Band Pass Frequency", NotificationType::sendNotification);
    lblFreqListHeader.setColour(Label::textColourId, Colour(200, 200, 200));
    lblFreqListHeader.setJustificationType(Justification::centred);
    lblFreqListHeader.setBounds(10, 10, 620, 50);
    addAndMakeVisible(lblFreqListHeader);

    // Initialize answer list
    int x = 10;
    int y = 70;
    int width = 80;
    int height = 40;

    Array<float> freqList = processor.bandPassGame.getFreqList();
    for (float freq: freqList) {
        TextButton *button = new TextButton(String((int) freq));
        button->setBounds(x, y, width, height);
        x += width + 10;
        button->onClick = [this, freq]() {
            processor.bandPassGame.setAnswer(freq);
            repaint();
        };
        btnFreqList.add(button);
        addAndMakeVisible(button);
    }

    // Initialize the start button
    btnStart.setButtonText("Start Training");
    btnStart.setBounds(10, 10, 620, 100);
    btnStart.setColour(TextButton::textColourOnId, Colour(200, 200, 200));
    btnStart.onClick = [this]() {
        processor.bandPassGame.start();
        repaint();
    };
    addAndMakeVisible(btnStart);

    // Initialize result
    lblResult.setBounds(10, 110, 620, 50);
    lblResult.setJustificationType(Justification::centred);
    addAndMakeVisible(lblResult);

    // Initialize summary
    lblSummary.setBounds(10, 250, 620, 50);
    lblSummary.setJustificationType(Justification::right);
    lblSummary.setColour(Label::textColourId, Colour(100, 100, 100));
    addAndMakeVisible(lblSummary);

    setSize(640, 300);
}

GoldenEarsEqAudioProcessorEditor::~GoldenEarsEqAudioProcessorEditor() {
    for (TextButton *button: btnFreqList) {
        delete button;
    }
}

//==============================================================================

float GoldenEarsEqAudioProcessorEditor::getWinningPct() {
    auto summary = processor.bandPassGame.getSummary();
    int totalWins = 0;
    int totalAttempts = 0;

    for (auto item: summary) {
        totalWins += item->totalWins;
        totalAttempts += item->totalAttempts;
    }

    if (totalAttempts == 0) {
        return 0;
    }

    return (totalWins / (float) (totalAttempts)) * 100;
}

void GoldenEarsEqAudioProcessorEditor::paint(Graphics &g) {
    g.fillAll(Colour(30, 30, 30));
    bool gameStarted = processor.bandPassGame.isStarted();

    // handle answer list rendering
    lblFreqListHeader.setVisible(gameStarted ? true : false);
    for (TextButton *button: btnFreqList) {
        button->setVisible(gameStarted ? true : false);
    }

    // handle start button rendering
    btnStart.setVisible(gameStarted ? false : true);

    // handle result rendering
    if (gameStarted) {
        lblResult.setVisible(false);
    } else {
        float correctAnswer = processor.bandPassGame.getCurrentFreq();
        float givenAnswer = processor.bandPassGame.getAnswer();
        bool isCorrect = correctAnswer == givenAnswer;
        String message = isCorrect ? "You are correct." : "It's " + String(correctAnswer) + ". (Your answer: " + String(givenAnswer) + ")";

        lblResult.setColour(Label::textColourId, isCorrect ? Colour(50, 200, 50) : Colour(200, 0, 0));
        if (correctAnswer == 0.0f) {
            lblResult.setVisible(false);
            lblSummary.setVisible(false);
        } else {
            lblResult.setText(message, NotificationType::sendNotification);
            lblResult.setVisible(true);
        }
    }

    // show the summary
    String summaryText = "Overall Correctness: " + String((int) getWinningPct()) + "%";
    lblSummary.setText(summaryText, NotificationType::sendNotification);
    lblSummary.setVisible(true);
}

void GoldenEarsEqAudioProcessorEditor::resized() {
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}
