/*
  ==============================================================================

    SynthVoice.h
    Created: 12 May 2020 8:51:52pm
    Author:  Francesco

  ==============================================================================
*/

#pragma once
#include<JuceHeader.h>
#include "SynthSound.h"

#define SAMPLE_RATE (44100)

//Oscillator base is an abstract class child of SynthVoice, define the methods to play and stop the oscillator
// we have two virtual methods: canPlaySound and renderWaveShape that are then defined in different ways from the children of the class

class OscillatorBase : public SynthesiserVoice  
{
public:
    OscillatorBase()
    {
        amplitude.reset(SAMPLE_RATE, 0.1);
        phaseIncrement.reset(SAMPLE_RATE, 0.1);
    }

    void startNote(int midiNoteNumber, float velocity, SynthesiserSound*, int) override
    {
        frequency = MidiMessage::getMidiNoteInHertz(midiNoteNumber);
        phaseIncrement.setTargetValue(((MathConstants<double>::twoPi) * frequency) / sampleRate);
        amplitude.setTargetValue(velocity);

        // Store the initial note and work out the maximum frequency deviations for pitch bend
        initialNote = midiNoteNumber;
        maxFreq = MidiMessage::getMidiNoteInHertz(initialNote + 4) - frequency;
        minFreq = frequency - MidiMessage::getMidiNoteInHertz(initialNote - 4);
    }

    void stopNote(float, bool) override
    {
        clearCurrentNote();
        amplitude.setTargetValue(0.0);
    }

    void pitchWheelMoved(int newValue) override
    {
        // Change the phase increment based on pitch bend amount
        auto frequencyOffset = ((newValue > 0 ? maxFreq : minFreq) * (newValue / 127.0));
        phaseIncrement.setTargetValue(((MathConstants<double>::twoPi) * (frequency + frequencyOffset)) / sampleRate);
    }

    void controllerMoved(int, int) override {}

    void channelPressureChanged(int newChannelPressureValue) override
    {
        // Set the amplitude based on pressure value
        amplitude.setTargetValue(newChannelPressureValue / 127.0);
    }

    void renderNextBlock(AudioBuffer<float>& outputBuffer, int startSample, int numSamples) override
    {
        while (--numSamples >= 0)
        {
            auto output = getSample() * amplitude.getNextValue();

            for (auto i = outputBuffer.getNumChannels(); --i >= 0;)
                outputBuffer.addSample(i, startSample, static_cast<float> (output));

            ++startSample;
        }
    }

    using SynthesiserVoice::renderNextBlock;

    /** Returns the next sample */
    double getSample()
    {
        auto output = renderWaveShape(phasePos);

        phasePos += phaseIncrement.getNextValue();

        if (phasePos > MathConstants<double>::twoPi)
            phasePos -= MathConstants<double>::twoPi;

        return output;
    }

    /** Subclasses should override this to say whether they can play the given sound */
    bool canPlaySound(SynthesiserSound*) override = 0;

    /** Subclasses should override this to render a waveshape */
    virtual double renderWaveShape(const double currentPhase) = 0;

private:
    SmoothedValue<double> amplitude, phaseIncrement;

    double frequency = 0.0;
    double phasePos = 0.0;
    double sampleRate = SAMPLE_RATE;

    int initialNote = 0;
    double maxFreq = 0.0, minFreq = 0.0;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OscillatorBase)
};


//we have 4 children: Sine , saw , triang and square. Each one of these defines the two methods in the proper way

struct SineVoice : public OscillatorBase
{
    SineVoice() {}

    bool canPlaySound(SynthesiserSound* sound) override { return dynamic_cast<SineSound*> (sound) != nullptr; }  //use to play with this voice only if we are using the sine sound

    double renderWaveShape(const double currentPhase) override { return sin(currentPhase); }

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SineVoice)
};

struct SquareVoice : public OscillatorBase
{
    SquareVoice() {}

    bool canPlaySound(SynthesiserSound* sound) override { return dynamic_cast<SquareSound*> (sound) != nullptr; }

    double renderWaveShape(const double currentPhase) override { return (currentPhase < MathConstants<double>::pi ? 0.0 : 1.0); }

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SquareVoice)
};

struct SawVoice : public OscillatorBase
{
    SawVoice() {}

    bool canPlaySound(SynthesiserSound* sound) override { return dynamic_cast<SawSound*> (sound) != nullptr; }

    double renderWaveShape(const double currentPhase) override { return (1.0 / MathConstants<double>::pi) * currentPhase - 1.0; }

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SawVoice)
};

struct TriangleVoice : public OscillatorBase
{
    TriangleVoice() {}

    bool canPlaySound(SynthesiserSound* sound) override { return dynamic_cast<TriangleSound*> (sound) != nullptr; }

    double renderWaveShape(const double currentPhase) override
    {
        return currentPhase < MathConstants<double>::pi ? -1.0 + (2.0 / MathConstants<double>::pi) * currentPhase
            : 3.0 - (2.0 / MathConstants<double>::pi) * currentPhase;
    }

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TriangleVoice)
};