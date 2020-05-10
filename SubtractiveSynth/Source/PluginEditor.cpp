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

    setSize (500, 350);

    waveformLabel.setText("Waveform :", dontSendNotification);
    amplitudeLabel.setText("Amplitude :", dontSendNotification);

    waveform.setRange(1, 5, 1);
    waveform.setSliderStyle(Slider::Rotary);
    waveform.setTextBoxStyle(Slider::TextBoxBelow, false, 100, 20);
    waveform.addListener(this);
    amplitude.setRange(0, 1, 0.05);
    amplitude.setSliderStyle(Slider::Rotary);
    amplitude.setTextBoxStyle(Slider::TextBoxBelow, false, 100, 20);
    amplitude.addListener(this);

    addAndMakeVisible(waveform);
    addAndMakeVisible(waveformLabel);
    addAndMakeVisible(amplitude);
    addAndMakeVisible(amplitudeLabel);


    filterKnob1.setRange(0, 10, 1);
    filterKnob1.setSliderStyle(Slider::Rotary);
    filterKnob1.setTextBoxStyle(Slider::TextBoxBelow, false, 80, 20);
    filterKnob1.addListener(this);
    addAndMakeVisible(filterKnob1);
    filterKnob2.setRange(0, 10, 1);
    filterKnob2.setSliderStyle(Slider::Rotary);
    filterKnob2.setTextBoxStyle(Slider::TextBoxBelow, false, 80, 20);
    filterKnob2.addListener(this);
    addAndMakeVisible(filterKnob2);
    filterKnob3.setRange(0, 10, 1);
    filterKnob3.setSliderStyle(Slider::Rotary);
    filterKnob3.setTextBoxStyle(Slider::TextBoxBelow, false, 80, 20);
    filterKnob3.addListener(this);
    addAndMakeVisible(filterKnob3);
    filterKnob4.setRange(0, 10, 1);
    filterKnob4.setSliderStyle(Slider::Rotary);
    filterKnob4.setTextBoxStyle(Slider::TextBoxBelow, false, 80, 20);
    filterKnob4.addListener(this);
    addAndMakeVisible(filterKnob4);
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

    FlexBox generalFlexBox;
    FlexBox secondaryFlex1;
    FlexBox secondaryFlex2;

    //FlexItem fb1(getWidth() / 2.0f, getHeight());
    //FlexItem fb2(getWidth() / 2.0f, getHeight());         // To have just 2 cells, see that later

    generalFlexBox.flexDirection = FlexBox::Direction::row;
    generalFlexBox.flexWrap = FlexBox::Wrap::wrap;
    generalFlexBox.justifyContent = FlexBox::JustifyContent::center;
    generalFlexBox.alignContent = FlexBox::AlignContent::center;

    secondaryFlex1.flexDirection = FlexBox::Direction::column;
    secondaryFlex1.flexWrap = FlexBox::Wrap::wrap;
    secondaryFlex1.justifyContent = FlexBox::JustifyContent::spaceAround;
    secondaryFlex1.alignContent = FlexBox::AlignContent::center;
    secondaryFlex2.flexDirection = FlexBox::Direction::column;
    secondaryFlex2.flexWrap = FlexBox::Wrap::wrap;
    secondaryFlex2.justifyContent = FlexBox::JustifyContent::spaceAround;
    secondaryFlex2.alignContent = FlexBox::AlignContent::center;

    secondaryFlex1.items = {FlexItem(100, 50, waveformLabel), FlexItem(100, 100, waveform)};
    secondaryFlex2.items = {FlexItem(100, 50, amplitudeLabel), FlexItem(100, 100, amplitude)};


    generalFlexBox.items = { FlexItem(150,150,secondaryFlex1), FlexItem(150,150,secondaryFlex2)};



    FlexBox filterFlexBox;
    filterFlexBox.flexDirection = FlexBox::Direction::row;
    filterFlexBox.flexWrap = FlexBox::Wrap::wrap;
    filterFlexBox.justifyContent = FlexBox::JustifyContent::spaceAround;
    filterFlexBox.alignContent = FlexBox::AlignContent::center;
    filterFlexBox.items = { FlexItem(100,100,filterKnob1), FlexItem(100,100,filterKnob2), FlexItem(100,100,filterKnob3), FlexItem(100,100,filterKnob4) };


    FlexBox mainFlexBox;
    mainFlexBox.flexDirection = FlexBox::Direction::column;
    mainFlexBox.flexWrap = FlexBox::Wrap::wrap;
    mainFlexBox.justifyContent = FlexBox::JustifyContent::spaceAround;
    mainFlexBox.alignContent = FlexBox::AlignContent::center;

    mainFlexBox.items = { FlexItem(400,150,generalFlexBox), FlexItem(400,150,filterFlexBox) };


    mainFlexBox.performLayout(bounds);

}

void SubtractiveSynthAudioProcessorEditor::sliderValueChanged(Slider * slider) {
    if (slider == &waveform) {
        //processor.setFreq(freq.getValue());
        DBG("dbg : Waveform Changed");
    }
    else if (slider == &amplitude) {
        DBG("dbg : Amplitude Changed");
        //processor.setAmplitude(amplitude.getValue());
    }
}
