/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include<juce_dsp/juce_dsp.h>
#include "SynthVoice.h"

#define SAMPLE_RATE (44100)
#ifndef M_PI
#define M_PI (3.14159265)
#endif

//==============================================================================
/**
*/
class SubtractiveSynthAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    SubtractiveSynthAudioProcessor();
    ~SubtractiveSynthAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    //setter

    void setCutOffFreq(float newcutOffFreq);
    void setResonance(float newResonance);
    void setWaveFormNum(float newWaveNum);
    void setOscAmplitude(float newAmplitude);

private:

    Synthesiser synth;  

    dsp::Gain<float> amplitude;    // amplitude of the oscillator
    float oscillatorFreq;   //freq of the oscillator
    float cutOffFreq;  //cut off freq of the filter
    float resonance;   // resonance of the filter
    int waveFormNum;   // number of the waveForm of the oscillator (from 0 to 3)

    dsp::ProcessorDuplicator<dsp::IIR::Filter <float>, dsp::IIR::Coefficients <float>> lowPassFilter;   // filter

    void updateFilter(); //use to apdate the parameters of the filter
    void addVoiceSynth(SynthesiserVoice* const newVoice);  // add a single voice to the synth, newVoice can be of classes: SineVoice, SquareVoice, TriangleVoice, SawVoice
    void addSoundSynth(); //add the 4 different sound to the synth, used only at the start

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SubtractiveSynthAudioProcessor)
};
