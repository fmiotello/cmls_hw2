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
    ), lowPassFilter(dsp::IIR::Coefficients<float>::makeLowPass(SAMPLE_RATE, 10.0f, 0.1f)), synth()
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
    waveFormNum = 2;

    dsp::ProcessSpec spec;        //set the ProcessSpec
    spec.sampleRate = SAMPLE_RATE;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = getTotalNumOutputChannels();

    amplitude.setGainDecibels(-6.0f);  //set amplitude and prepare filter
    amplitude.prepare(spec);
    lowPassFilter.prepare(spec);
    lowPassFilter.reset();

    //add the 4 prototype to the class OscillatorBase, this is done to dinamically generate new voice during the execution
    OscillatorBase::addPrototype(1, std::unique_ptr<OscillatorBase>(new SineVoice()));
    OscillatorBase::addPrototype(2, std::unique_ptr<OscillatorBase>(new SawVoice()));
    OscillatorBase::addPrototype(3, std::unique_ptr<OscillatorBase>(new TriangleVoice()));
    OscillatorBase::addPrototype(4, std::unique_ptr<OscillatorBase>(new SquareVoice()));

    synth.setCurrentPlaybackSampleRate(SAMPLE_RATE);  // set the synth
    addSoundSynth();
}

void SubtractiveSynthAudioProcessor::addSoundSynth() { 
    synth.clearSounds();
    synth.clearVoices();

    //add all the possible sounds to the synth
    synth.addSound(new SineSound());    
    synth.addSound(new SquareSound());
    synth.addSound(new SawSound());
    synth.addSound(new TriangleSound());
}

void SubtractiveSynthAudioProcessor::addVoiceSynth(int newWaveNum) {  // this method add a single voice to the synth. you have to pass the number that you want

    synth.addVoice(OscillatorBase::makeProduct(newWaveNum));
}

void SubtractiveSynthAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
    OscillatorBase::freeResource();
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
    if(newWaveNum > 0) waveFormNum = newWaveNum;
    synth.clearVoices();   // clear all the voice to add the new voice based on the waveForm num
    addVoiceSynth(newWaveNum);
}

void SubtractiveSynthAudioProcessor::setOscAmplitude(float newAmplitude) {
    amplitude.setGainLinear(newAmplitude);
}

//=================================================================================

void SubtractiveSynthAudioProcessor::updateFilter() { //update the filter coefficent basend on cutoff and resonance
    *lowPassFilter.state = *dsp::IIR::Coefficients<float>::makeLowPass(SAMPLE_RATE, cutOffFreq, resonance);
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

    synth.renderNextBlock(tempBuffer, MidiBuffer() , 0, numSamples);  //synthesis of the wave

    dsp::AudioBlock<float> currentBlock(tempBuffer);        // create an audio block that points to the tempBuffer filled with the wave samples

    updateFilter(); //update the filter parameter
    lowPassFilter.process(dsp::ProcessContextReplacing <float>(currentBlock));  //filtering the current block
    amplitude.process(dsp::ProcessContextReplacing <float>(currentBlock));  //apply the gain to the block
    currentBlock.copyTo(buffer); //copy the block after all the processing to the output buffer
    
    for (MidiBuffer::Iterator i(midiMessages); i.getNextEvent(m, time);)       //listening for midi event and change the freq of the oscillator
        if (m.isNoteOn()) {
            addVoiceSynth(waveFormNum);
            synth.noteOn(waveFormNum, m.getNoteNumber(), 0.1);  // the first parameter of noteOn is used to select the midiChannel (we have one channel for every sound, 4 in our case)
        }
        else if (m.isNoteOff()) 
        {
            synth.noteOff(waveFormNum, m.getNoteNumber(), 1, false);
            synth.removeVoice(synth.getNumVoices()-1);
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
