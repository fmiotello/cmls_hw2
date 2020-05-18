
#pragma once
#include<JuceHeader.h>


//4 different sound that are applied to 4 different channels
//these sound are used to select what type of voice plays. If I select the channel 1 only the sine voice can play...

struct SineSound : public SynthesiserSound
{
    SineSound() {}

    bool appliesToNote(int) override { return true; }

    bool appliesToChannel(int midiChannel) override { return (midiChannel == 1); }  

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SineSound)
};

struct SawSound : public SynthesiserSound
{
    SawSound() {}

    bool appliesToNote(int) override { return true; }

    bool appliesToChannel(int midiChannel) override { return (midiChannel == 2); }

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SawSound)
};

struct TriangleSound : public SynthesiserSound
{
    TriangleSound() {}

    bool appliesToNote(int) override { return true; }

    bool appliesToChannel(int midiChannel) override { return (midiChannel == 3); }

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TriangleSound)
};

struct SquareSound : public SynthesiserSound
{
    SquareSound() {}

    bool appliesToNote(int) override { return true; }

    bool appliesToChannel(int midiChannel) override { return (midiChannel == 4); }

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SquareSound)
};


