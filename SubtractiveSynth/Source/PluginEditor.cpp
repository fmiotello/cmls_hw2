/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SubtractiveSynthAudioProcessorEditor::SubtractiveSynthAudioProcessorEditor (SubtractiveSynthAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{

    setSize (400, 250);

    waveformLabel.setText("Waveform :", dontSendNotification);
    filterLabel.setText("Filter(?) :", dontSendNotification);
    waveform.setRange(1, 5, 1);
    waveform.setSliderStyle(Slider::Rotary);
    waveform.setTextBoxStyle(Slider::TextBoxBelow, false, 100, 20);
    waveform.addListener(this);
    filter.setRange(0, 2, 1);
    filter.setSliderStyle(Slider::Rotary);
    filter.setTextBoxStyle(Slider::TextBoxBelow, false, 100, 20);
    filter.addListener(this);
    addAndMakeVisible(waveform);
    addAndMakeVisible(waveformLabel);
    addAndMakeVisible(filter);
    addAndMakeVisible(filterLabel);
}

SubtractiveSynthAudioProcessorEditor::~SubtractiveSynthAudioProcessorEditor()
{
}

//==============================================================================
void SubtractiveSynthAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    g.setColour (Colours::white);
    g.setFont (15.0f);
    //g.drawFittedText ("Hello World!", getLocalBounds(), Justification::centred, 1);
}

void SubtractiveSynthAudioProcessorEditor::resized()
{
    Rectangle<int> bounds = getLocalBounds();

    FlexBox mainFlexBox;

    //FlexItem fb1(getWidth() / 2.0f, getHeight());
    //FlexItem fb2(getWidth() / 2.0f, getHeight());         // To have just 2 cells, see that later

    mainFlexBox.flexDirection = FlexBox::Direction::row;
    mainFlexBox.flexWrap = FlexBox::Wrap::wrap;
    mainFlexBox.justifyContent = FlexBox::JustifyContent::spaceAround;
    mainFlexBox.alignContent = FlexBox::AlignContent::center;

    Array<FlexItem> mainItemArray;

    mainItemArray.add(FlexItem(65, 50, waveformLabel));
    mainItemArray.add(FlexItem(100, 100, waveform));
    mainItemArray.add(FlexItem(65, 50, filterLabel));
    mainItemArray.add(FlexItem(100, 100, filter));

    mainFlexBox.items = mainItemArray;

    mainFlexBox.performLayout(bounds);


    //waveformLabel.setBounds(30, 50, 130, 20);
    //waveform.setBounds(30, 80, 100, 100);
    //filterLabel.setBounds(200, 50, 130, 20);
    //filter.setBounds(200, 80, 100, 100);
}

void SubtractiveSynthAudioProcessorEditor::sliderValueChanged(Slider * slider) {
    if (slider == &waveform) {
        //processor.setFreq(freq.getValue());
        DBG("dbg : Waveform Changed");
    }
    else if (slider == &filter) {
        DBG("dbg : Filter Changed");
        //processor.setAmplitude(amplitude.getValue());
    }
}
