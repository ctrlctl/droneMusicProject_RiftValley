/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DroneProjectAudioProcessor::DroneProjectAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
}

DroneProjectAudioProcessor::~DroneProjectAudioProcessor()
{
}

//==============================================================================
const juce::String DroneProjectAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool DroneProjectAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool DroneProjectAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool DroneProjectAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double DroneProjectAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int DroneProjectAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int DroneProjectAudioProcessor::getCurrentProgram()
{
    return 0;
}

void DroneProjectAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String DroneProjectAudioProcessor::getProgramName (int index)
{
    return {};
}

void DroneProjectAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void DroneProjectAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    squareOsc.setSampleRate(sampleRate);
    squareOsc.setFrequency(200.0f);

    gainVolFs.setSampleRate(sampleRate);
    gainVolFs.setFrequency(0.008f);

    gainVolFm.setSampleRate(sampleRate);
    gainVolFm.setFrequency(0.008f);

    lfo.setSampleRate(sampleRate);
    lfo.setFrequency(0.05f);

    filterSynth.setSampleRate(sampleRate);
    filterSynth.setLfoFrequency(0.1f);

    fmSynth.setSampleRate(sampleRate);

    data.prepareFilterSynth(sampleRate);
    data.prepareFMSynth(sampleRate);

    sr = sampleRate;

    filter.setCoefficients(juce::IIRCoefficients::makeLowPass(sampleRate, 4000.0f, 5.0f));
    filter.reset();

    juce::Reverb::Parameters reverbParams;
    reverbParams.dryLevel = 0.5f;
    reverbParams.wetLevel = 0.5f;
    reverbParams.roomSize = 0.99f;

    reverb.setParameters(reverbParams);
    reverb.reset();

    
    delay.setSize(sampleRate * 2);
    delay.setDelayTime(0.25 * sampleRate);

    panningLfo.setSampleRate(sampleRate);
    panningLfo.setFrequency(0.25);
    
}

void DroneProjectAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool DroneProjectAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
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

void DroneProjectAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    int numSamples = buffer.getNumSamples();

    // pointers to audio arrays
    auto* leftChannel = buffer.getWritePointer(0); //int channelNumber
    auto* rightChannel = buffer.getWritePointer(1);

    // DSP Loop!

    for (int i = 0; i < numSamples; i++) {
        //float noise = 0.5 * random.nextFloat();  // random num between 0 and 1
        //float env = squareVol.process() + 0.5f;
        //float sample = noise * env;
        // float sample = ((rand() % 1000) / 10000.0f) - 0.5;
        /*float cutoff = lfo.process() * 330.0f + 440.0f;
        filter.setCoefficients(juce::IIRCoefficients::makeLowPass(sr, cutoff, 5.0f));
        float rawSquareWave = squareOsc.process() * 0.9f;
        float sample = filter.processSingleSampleRaw(rawSquareWave);*/
        //float sample = squareOsc.process() * 0.9f;
        // leftChannel[i] = sample;
        // rightChannel[i] = sample;
        // ======= Filtered Square wave =======

        //float noise = 0.5 * random.nextFloat();  // random num between 0 and 1
        //float env = gainVol.process()+1.0f;
        //float noise_output = 0.01f * noise * env;
        //// low pass sin
        //float filteredSynthOut0 = 0.05*filterSynth.processFilteredSin(0,55);

        // low pass saw
        // high pass square
        //float filteredSynthOut1 = filterSynth.processFilteredSquare(1, 5);

        //// FM Tri
        //float fmSynthOut = 0.08 * fmSynth.processFMTri(80, 100.0f);

        //// FM Square : elephant
        ////kid
        //float fmSynthOut1 = 0.04 * fmSynth.processFMSaw(70, 200.0f);
        //// mom
        //float fmSynthOut2 = 0.04 * fmSynth.processFMSaw(55, 100.0f);
        
        // FM Saw gain env
        
        //float env = gainVol.process() + 0.5f;
        
        //float mix = (filteredSynthOut0 + fmSynthOut + noise_output) * 0.3f;

        ////float mix = (fmSynthOut2 ) * 0.3f;
        //leftChannel[i] = 0.125f * mix;
        //rightChannel[i] = 0.125f * mix;


        
        // set up parameters for delay and panning
        float panVal = panningLfo.process();
        float delayMod = panningLfo.process();

        float delayTime = delayMod * 200 + 5000;
        delay.setDelayTime(delayTime);
        float leftGain = (panVal * 0.5f) + 0.5f;  // 0 - 1
        float rightGain = 1.0f - leftGain;         // 1 - 0

        float env_fm = gainVolFm.process();
        float env_fs = gainVolFs.process();
        if (env_fm < 0)
            env_fm *= 0.01f;
        else
            env_fs *= 0.2f;
        //// process FM Synth using delay
        float sample_fm = data.processFMSynth();
        float delayedSample = 0.3 * delay.process(sample_fm);

        // process filter Synth
        float sample_fs = data.processFilterSynth();

        leftChannel[i] = ((delayedSample + sample_fm) * env_fm + sample_fs * env_fs) * leftGain * 0.5f;
        rightChannel[i] = ((delayedSample + sample_fm) * env_fm + sample_fs * env_fs) * rightGain * 0.5f;

       /* leftChannel[i] = (sample_fs * env_fs) * leftGain * 0.5f;
        rightChannel[i] = (sample_fs * env_fs) * rightGain * 0.5f;*/

        /*leftChannel[i] = sample_fm * env * 0.5f + sample_fs * 0.5f;
        rightChannel[i] = sample_fm * env * 0.5f + sample_fs * 0.5f;*/
        
    }

    reverb.processStereo(leftChannel, rightChannel, numSamples);

    //filter.setCoefficients(juce::IIRCoefficients(juce::IIRCoefficients::makeLowPass(sr, lfoVal*330.0f)));
}

//==============================================================================
bool DroneProjectAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* DroneProjectAudioProcessor::createEditor()
{
    return new DroneProjectAudioProcessorEditor (*this);
}

//==============================================================================
void DroneProjectAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void DroneProjectAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new DroneProjectAudioProcessor();
}
