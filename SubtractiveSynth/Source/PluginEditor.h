/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class SubtractiveSynthAudioProcessorEditor  : public AudioProcessorEditor, private Slider::Listener
{
public:
    SubtractiveSynthAudioProcessorEditor (SubtractiveSynthAudioProcessor&);
    ~SubtractiveSynthAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    SubtractiveSynthAudioProcessor& processor;

    Slider waveform;
    Label waveformLabel;
    Slider amplitude;
    Label amplitudeLabel;

    Slider filterKnob1;
    Slider filterKnob2;
    Slider filterKnob3;
    Slider filterKnob4;

    void sliderValueChanged(Slider* slider) override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SubtractiveSynthAudioProcessorEditor)
};
