/*
  ==============================================================================

    SynthData.h
    Created: 16 Mar 2022 9:37:20am
    Author:  lancelotli

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Adsr.h"
#include "FilterSynth.h"
#include "FMSynth.h"

//==============================================================================
/*
*/
class SynthData
{
public:
    SynthData();
    ~SynthData();
    void prepareFilterSynth(double sampleRate);
    void prepareFMSynth(double sampleRate);
    float processFilterSynth();
    float processFMSynth();

private:
    std::vector<FilterSynth> filterSynth;
    std::vector<FMSynth> fmSynth;
    juce::IIRFilter filter;
    double sr;
    SinOsc lfo_fm_filter;
    SinOsc lfo_fm_gain;

};
