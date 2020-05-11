/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SubtractiveSynthAudioProcessor::SubtractiveSynthAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
        .withInput("Input", AudioChannelSet::stereo(), true)
#endif
        .withOutput("Output", AudioChannelSet::stereo(), true)
#endif
    ), lowPassFilter(dsp::IIR::Coefficients<float>::makeLowPass(SAMPLE_RATE, 10.0f, 0.1f))
#endif
{
}

SubtractiveSynthAudioProcessor::~SubtractiveSynthAudioProcessor()
{
}

//==============================================================================
const String SubtractiveSynthAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool SubtractiveSynthAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool SubtractiveSynthAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool SubtractiveSynthAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double SubtractiveSynthAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int SubtractiveSynthAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int SubtractiveSynthAudioProcessor::getCurrentProgram()
{
    return 0;
}

void SubtractiveSynthAudioProcessor::setCurrentProgram (int index)
{
}

const String SubtractiveSynthAudioProcessor::getProgramName (int index)
{
    return {};
}

void SubtractiveSynthAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void SubtractiveSynthAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    oscillatorFreq = 440.0f;   //set some default value
    cutOffFreq = 0.1f;
    resonance = 0.1f;
    waveFormNum = 0;
    oscillatorGain.setGainDecibels(-6.0f);

    dsp::ProcessSpec spec;        //set the ProcessSpec
    spec.sampleRate = SAMPLE_RATE;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = getTotalNumOutputChannels();

    for (auto&& oscillator : oscArray) {
        oscillator.setFrequency(oscillatorFreq);
        oscillator.prepare(spec);
    }

    lowPassFilter.prepare(spec);
    lowPassFilter.reset();
}

void SubtractiveSynthAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool SubtractiveSynthAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

//====================================================================================
//setter

void SubtractiveSynthAudioProcessor::setCutOffFreq(float newcutOffFreq) { //newCutOffFreq different from 0
    if(newcutOffFreq > 0) cutOffFreq = newcutOffFreq;  
}

void SubtractiveSynthAudioProcessor::setResonance(float newResonance) {  //newResonance different from 0
    if(resonance > 0) resonance = newResonance;
}

void SubtractiveSynthAudioProcessor::setWaveFormNum(float newWaveNum) {  //newWaveNum number goes from 1 to 4
    if(newWaveNum > 0) waveFormNum = newWaveNum-1;
}

void SubtractiveSynthAudioProcessor::setOscAmplitude(float newAmplitude) {
    oscillatorGain.setGainLinear(newAmplitude);
}

//=================================================================================

void SubtractiveSynthAudioProcessor::updateFilter() {
    *lowPassFilter.state = *dsp::IIR::Coefficients<float>::makeLowPass(SAMPLE_RATE, cutOffFreq, resonance);
    //*lowPassFilter.state = *dsp::FilterDesign<float>::designIIRHighpassHighOrderButterworthMethod(cutOffFreq, SAMPLE_RATE, 4)[0];
}

void SubtractiveSynthAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    int time;
    MidiMessage m;
    int numSamples = buffer.getNumSamples();
    AudioBuffer<float> tempBuffer;

    tempBuffer.setSize(totalNumOutputChannels, numSamples);
    tempBuffer.clear();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    dsp::AudioBlock<float> currentBlock(tempBuffer);
    oscArray[waveFormNum].process(dsp::ProcessContextReplacing <float>(currentBlock));    //oscillator.process put in the current block the value of the wave
    oscillatorGain.process(dsp::ProcessContextReplacing <float>(currentBlock));     //apply che gain to the current block

    updateFilter(); //update the filter parameter
    lowPassFilter.process(dsp::ProcessContextReplacing <float>(currentBlock));  //filtering the current block
    currentBlock.copyTo(buffer); //copy the block after all the processing to the output buffer
    
    for (MidiBuffer::Iterator i(midiMessages); i.getNextEvent(m, time);)       //listening for midi event and change the freq of the oscillator
        if (m.isNoteOn()) {
            oscillatorFreq = MidiMessage::getMidiNoteInHertz(m.getNoteNumber()); //frequency in hertz
            for (auto&& oscillator : oscArray)
                oscillator.setFrequency(oscillatorFreq);
        } 
}

//==============================================================================
bool SubtractiveSynthAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* SubtractiveSynthAudioProcessor::createEditor()
{
    return new SubtractiveSynthAudioProcessorEditor(*this);
}

//==============================================================================
void SubtractiveSynthAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void SubtractiveSynthAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SubtractiveSynthAudioProcessor();
}
