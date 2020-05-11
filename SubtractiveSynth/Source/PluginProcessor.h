/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include<juce_dsp/juce_dsp.h>

#define NUM_SECONDS (100)
#define SAMPLE_RATE (44100)
#define FRAMES_PER_BUFFER (256) // 64
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
    void setCutOffFreq(float newcutOffFreq);
    void setResonance(float newResonance);
    void setWaveFormNum(float newWaveNum);
    void setOscAmplitude(float newAmplitude);

private:

    dsp::Gain<float> oscillatorGain;
    float oscillatorFreq;   //freq of the oscillator
    float cutOffFreq;  //cut off freq of the filter
    float resonance;   // resonance of the filter
    int waveFormNum;   // number of the waveForm of the oscillator (from 0 to 3)

    dsp::ProcessorDuplicator<dsp::IIR::Filter <float>, dsp::IIR::Coefficients <float>> lowPassFilter;   // filter
    dsp::Oscillator<float> oscArray[4] =     //array of Oscillator, each element is an oscillator with a specific waveform
    {
        // No Approximation
        {[](float x) { return std::sin(x); }},                   // sine
        {[](float x) { return x / MathConstants<float>::pi; }},   // saw
        {[](float x) { return x < 0.0f ? -1.0f : 1.0f; }},        // square

        {[](float x) { return x < MathConstants<double>::pi ? -1.0 + (2.0 / MathConstants<double>::pi) * x
                                                            : 3.0 - (2.0 / MathConstants<double>::pi) * x; }}, // triang


        //// Approximated by a wave-table
        //{[](float x) { return std::sin(x); }, 100},                 // sine
        //{[](float x) { return x / MathConstants<float>::pi; }, 100}, // saw
        //{[](float x) { return x < 0.0f ? -1.0f : 1.0f; }, 100}       // square
    };

    void updateFilter();

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SubtractiveSynthAudioProcessor)
};
