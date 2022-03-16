/*
  ==============================================================================

    Adsr.h
    Created: 16 Mar 2022 9:36:01am
    Author:  lancelotli

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class Adsr : public juce::ADSR
{
public:
    void update(const float attack, const float decay, const float sustain, const float release)
    {
        adsrParams.attack = attack;
        adsrParams.decay = decay;
        adsrParams.sustain = sustain;
        adsrParams.release = release;

        setParameters(adsrParams);
    }

private:
    juce::ADSR::Parameters adsrParams;
};
