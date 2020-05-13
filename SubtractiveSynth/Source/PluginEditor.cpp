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

    setSize (500, 450);

    waveformLabel.setText("Waveform :", dontSendNotification);
    amplitudeLabel.setText("Amplitude :", dontSendNotification);

    waveform.setRange(1, 4, 1);
    waveform.setSliderStyle(Slider::Rotary);
    waveform.setTextBoxStyle(Slider::TextBoxBelow, false, 100, 20);
    waveform.addListener(this);
    waveform.setValue(2);
    amplitude.setRange(0, 1, 0.05);
    amplitude.setSliderStyle(Slider::Rotary);
    amplitude.setTextBoxStyle(Slider::TextBoxBelow, false, 100, 20);
    amplitude.addListener(this);
    amplitude.setValue(0.7);

    addAndMakeVisible(waveform);
    addAndMakeVisible(waveformLabel);
    addAndMakeVisible(amplitude);
    addAndMakeVisible(amplitudeLabel);


    cutOffFreqLabel.setText("Cut-off freq :", dontSendNotification);
    resonanceLabel.setText("Resonance :", dontSendNotification);
    poleNumberLabel.setText("Number of poles :", dontSendNotification);

    cutOffFreq.setRange(1000, 10000, 1);    //maybe we will use setNormalisableRange (NormalisableRange< double > newNormalisableRange) to use a logarithmic scale.
    cutOffFreq.setSliderStyle(Slider::Rotary);
    cutOffFreq.setTextBoxStyle(Slider::TextBoxBelow, false, 80, 20);
    cutOffFreq.addListener(this);
    addAndMakeVisible(cutOffFreq);
    cutOffFreq.setValue(2000);
    cutOffFreq.setTextValueSuffix("Hz");

    resonance.setRange(1, 25, 0.5);
    resonance.setSliderStyle(Slider::Rotary);
    resonance.setTextBoxStyle(Slider::TextBoxBelow, false, 80, 20);
    resonance.addListener(this);
    addAndMakeVisible(resonance);
    resonance.setValue(10);

    poleNumber.setRange(1, 4, 1);
    poleNumber.setSliderStyle(Slider::Rotary);
    poleNumber.setTextBoxStyle(Slider::TextBoxBelow, false, 80, 20);
    poleNumber.addListener(this);
    addAndMakeVisible(poleNumber);
    poleNumber.setValue(2);

    addAndMakeVisible(cutOffFreq);
    addAndMakeVisible(cutOffFreqLabel);
    addAndMakeVisible(resonance);
    addAndMakeVisible(resonanceLabel);
    addAndMakeVisible(poleNumber);
    addAndMakeVisible(poleNumberLabel);


    Font mainFont = Font(18, Font::bold);
    labelMainPart.setText("main :", dontSendNotification);
    labelMainPart.setFont(mainFont);
    addAndMakeVisible(labelMainPart);
    labelFilterPart.setText("filter :", dontSendNotification);
    labelFilterPart.setFont(mainFont);
    addAndMakeVisible(labelFilterPart);
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
    secondaryFlex1.justifyContent = FlexBox::JustifyContent::spaceAround;
    secondaryFlex1.alignContent = FlexBox::AlignContent::center;
    secondaryFlex2.flexDirection = FlexBox::Direction::column;
    secondaryFlex2.justifyContent = FlexBox::JustifyContent::spaceAround;
    secondaryFlex2.alignContent = FlexBox::AlignContent::center;

    secondaryFlex1.items = {FlexItem(100, 15, waveformLabel), FlexItem(100, 100, waveform)};
    secondaryFlex2.items = {FlexItem(100, 15, amplitudeLabel), FlexItem(100, 100, amplitude)};

    generalFlexBox.items = { FlexItem(150,150,secondaryFlex1), FlexItem(150,150,secondaryFlex2)};



    FlexBox filterFlexBox;
    FlexBox secondaryFlex3;
    FlexBox secondaryFlex4;
    FlexBox secondaryFlex5;


    filterFlexBox.flexDirection = FlexBox::Direction::row;
    filterFlexBox.flexWrap = FlexBox::Wrap::wrap;
    filterFlexBox.justifyContent = FlexBox::JustifyContent::spaceAround;
    filterFlexBox.alignContent = FlexBox::AlignContent::center;

    secondaryFlex3.flexDirection = FlexBox::Direction::column;
    secondaryFlex3.justifyContent = FlexBox::JustifyContent::spaceAround;
    secondaryFlex3.alignContent = FlexBox::AlignContent::center;
    secondaryFlex4.flexDirection = FlexBox::Direction::column;
    secondaryFlex4.justifyContent = FlexBox::JustifyContent::spaceAround;
    secondaryFlex4.alignContent = FlexBox::AlignContent::center;
    secondaryFlex5.flexDirection = FlexBox::Direction::column;
    secondaryFlex5.justifyContent = FlexBox::JustifyContent::spaceAround;
    secondaryFlex5.alignContent = FlexBox::AlignContent::center;

    secondaryFlex3.items = { FlexItem(100, 15, cutOffFreqLabel), FlexItem(100, 100, cutOffFreq) };
    secondaryFlex4.items = { FlexItem(100, 15, resonanceLabel), FlexItem(100, 100, resonance) };
    secondaryFlex5.items = { FlexItem(100, 15, poleNumberLabel), FlexItem(100, 100, poleNumber) };

    filterFlexBox.items = { FlexItem(100,150,secondaryFlex3), FlexItem(100,150,secondaryFlex4), FlexItem(100,150,secondaryFlex5)};


    FlexBox mainFlexBox;
    mainFlexBox.flexDirection = FlexBox::Direction::column;
    mainFlexBox.flexWrap = FlexBox::Wrap::wrap;
    mainFlexBox.justifyContent = FlexBox::JustifyContent::spaceAround;
    mainFlexBox.alignContent = FlexBox::AlignContent::center;

    mainFlexBox.items = { FlexItem(100,15,labelMainPart), FlexItem(400,150,generalFlexBox),FlexItem(400,40,Label(" ")), FlexItem(100,15,labelFilterPart), FlexItem(400,150,filterFlexBox) };


    mainFlexBox.performLayout(bounds);

}

void SubtractiveSynthAudioProcessorEditor::sliderValueChanged(Slider * slider) {
    if (slider == &waveform) {
        //processor.setFreq(freq.getValue());
        DBG("dbg : Waveform Changed");
        int valueWav = waveform.getValue();
        if (valueWav == 1) {
            waveform.setTextValueSuffix(": sine");
            processor.setWaveFormNum(valueWav);
        }
        else if (valueWav == 2) {
            waveform.setTextValueSuffix(": sawtooth");
            processor.setWaveFormNum(valueWav);
        }
        else if (valueWav == 3) {
            waveform.setTextValueSuffix(": triangular");
            processor.setWaveFormNum(valueWav);
        }
        else if (valueWav == 4) {
            waveform.setTextValueSuffix(": square");
            processor.setWaveFormNum(valueWav);
        }
        else {
            waveform.setTextValueSuffix(": ERROR");
        }
    }
    else if (slider == &amplitude) {
        DBG("dbg : Amplitude Changed");
        processor.setOscAmplitude(amplitude.getValue());
    }
    else if (slider == &cutOffFreq) {
        DBG("dbg : Cut-off freq Changed");
        processor.setCutOffFreq(cutOffFreq.getValue());
    }
    else if (slider == &resonance) {
        DBG("dbg : Resonance Changed");
        processor.setResonance(resonance.getValue());
    }
    else if (slider == &poleNumber) {
        DBG("dbg : Pole number Changed");
        //processor.setPoleNumber(poleNumber.getValue());
    }
}
