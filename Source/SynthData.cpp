/*
  ==============================================================================

    SynthData.cpp
    Created: 16 Mar 2022 9:37:20am
    Author:  lancelotli

  ==============================================================================
*/

#include <JuceHeader.h>
#include "SynthData.h"

//==============================================================================
SynthData::SynthData()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

}

SynthData::~SynthData()
{
    delete[] & filterSynth;
    delete[] & fmSynth;
}

void SynthData::prepareFilterSynth(double sampleRate)
{
    sr = sampleRate;
    
    filterSynth.push_back(FilterSynth());
    filterSynth[0].setSampleRate(sampleRate);
    filterSynth[0].setLfoFrequency(0.01f);

    filterSynth.push_back(FilterSynth());
    filterSynth[1].setSampleRate(sampleRate);
    filterSynth[1].setLfoFrequency(0.01f);

    filterSynth.push_back(FilterSynth());
    filterSynth[2].setSampleRate(sampleRate);
    filterSynth[2].setLfoFrequency(0.1f);
}

void SynthData::prepareFMSynth(double sampleRate)
{
    sr = sampleRate; 
    lfo_fm_filter.setSampleRate(sampleRate);
    lfo_fm_filter.setFrequency(0.1f);
    lfo_fm_gain.setSampleRate(sampleRate);
    lfo_fm_gain.setFrequency(0.1f);

    fmSynth.push_back(FMSynth());
    fmSynth[0].setSampleRate(sampleRate);

    fmSynth.push_back(FMSynth());
    fmSynth[1].setSampleRate(sampleRate);

    fmSynth.push_back(FMSynth());
    fmSynth[2].setSampleRate(sampleRate);
    
}

float SynthData::processFilterSynth()
{
    float filterSynthOut[3]; 
    
    // low pass sine wave
    filterSynthOut[0] = 0.05 * filterSynth[0].processFilteredSin(0, 55);
    filterSynthOut[1] = 5*  filterSynth[1].processFilteredSaw(2, 50);
    filterSynthOut[2] = 0.005 * filterSynth[2].processFilteredSquare(1, 59,sr);
    
    float output = filterSynthOut[0] +filterSynthOut[1] + filterSynthOut[2];
    return output;
}

float SynthData::processFMSynth()
{
    float lfo_fm_Val = lfo_fm_filter.process();
    float cutoff = (lfo_fm_Val * 900 + 1100) / 6;
    float Q = (lfo_fm_Val * 150.0f + 160.0f) / 4;
    filter.setCoefficients(juce::IIRCoefficients::makeLowPass(sr, cutoff, Q));

    // initialize an arry for FM Synth Output
    float fmSynthOut[3];
    fmSynthOut[0] = 0.08 * fmSynth[0].processFMTri(59, 100.0f);     // saw tooth wav //80
    fmSynthOut[1] = 0.04 * filter.processSingleSampleRaw(fmSynth[1].processFMSaw(50, 200.0f));
    fmSynthOut[2] = 0.04 * filter.processSingleSampleRaw(fmSynth[2].processFMSaw(55, 100.0f));


    float output = fmSynthOut[0] + fmSynthOut[1] + fmSynthOut[2];
    return output;
}
