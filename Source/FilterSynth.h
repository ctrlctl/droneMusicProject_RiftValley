/*
  ==============================================================================

    FilterSynth.h
    Created: 14 Mar 2022 10:07:02am
    Author:  lancelotli

  ==============================================================================
*/

#pragma once

#include "Oscillators.h"
#include <JuceHeader.h>

class FilterSynth {
public:
    void setSampleRate(float sr) 
    {
        sampleRate = sr;
        sinOsc.setSampleRate(sr);
        triOsc.setSampleRate(sr);
        sawOsc.setSampleRate(sr);
        squareOsc.setSampleRate(sr);
        lfo_cutoff.setSampleRate(sr);
        lfo_Q.setSampleRate(sr);
    }

    void setLfoFrequency(float f)
    {
        lfo_cutoff.setFrequency(f);
        lfo_Q.setFrequency(f);
    }

    // process sine wave using choice of filter and frequency
    float processFilteredSin(int filterchoice, int midiNoteNumber)
    {
        sinOsc.setFrequency(juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber));
        float OscOut = sinOsc.process();
        filterSetup(filterchoice);
        float filteredWave = filter.processSingleSampleRaw(OscOut);
        return filteredWave;
    }

    // process triangle wave using choice of filter and frequency
    float processFilteredTri(int filterchoice, int midiNoteNumber)
    {
        triOsc.setFrequency(juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber));
        float OscOut = triOsc.process();
        filterSetup(filterchoice);
        float filteredWave = filter.processSingleSampleRaw(OscOut);
        return filteredWave;
    }

    // process sawtooth wave using choice of filter and frequency
    float processFilteredSaw(int filterchoice, int midiNoteNumber)
    {
        sawOsc.setFrequency(juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber));
        float OscOut = sawOsc.process();
        filterSetup(filterchoice);
        float filteredWave = filter.processSingleSampleRaw(OscOut);
        return filteredWave;
    }

    // process square wave using choice of filter and frequency
    float processFilteredSquare(int filterchoice, int midiNoteNumber, float sr)
    {
        squareOsc.setFrequency(juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber));
        //squareOsc.setPulseWidth(midiNoteNumber/12);
        float OscOut = squareOsc.process();
        filterSetup(filterchoice);
        float filteredWave = filter.processSingleSampleRaw(OscOut);
        return filteredWave;
    }

    // Setting up dynamic filter
    // filter : 0 -- low pass
    //          1 -- high pass
    //          2 -- band pass
    void filterSetup(int filterchoice)   
    {
        float lfo_cutoff_Val = lfo_cutoff.process();;
        float lfo_Q_Val = lfo_Q.process();
        float cutoff;
        float Q;

        /// set up filter using choices
        switch(filterchoice)
        {
            // low pass filter
            case 0:
                if (lfo_cutoff_Val >= 0.5)
                    lfo_cutoff_Val /= 6;
                cutoff = (lfo_cutoff_Val * 900 + 1100) / 6;
                Q = (lfo_Q_Val * 150.0f + 170.0f) / 4;
                filter.setCoefficients(juce::IIRCoefficients::makeLowPass(sampleRate, cutoff, Q));
                break;

            // high pass filter
            case 1:
                cutoff = (lfo_cutoff_Val * 500 + 600) / 6;
                Q = (lfo_Q_Val * 500.0f + 510.0f) / 4;
                filter.setCoefficients(juce::IIRCoefficients::makeHighPass(sampleRate, cutoff, Q));
                break;

            // band pass filter
            case 2:
                cutoff = (lfo_cutoff_Val * 2000 + 2100) / 3;
                Q = (lfo_Q_Val * 500.0f + 510.0f) / 4;
                filter.setCoefficients(juce::IIRCoefficients::makeBandPass(sampleRate, cutoff, Q));
                break;
            
            default:
                jassertfalse;   // You're not supposed to be here
                break;
        }    
    }

private:
    // Original waves
    SinOsc sinOsc;
    TriOsc triOsc;
    SawOsc sawOsc;
    SquareOsc squareOsc;

    // Lfo for modulation
    SinOsc lfo_cutoff;     // lfo for cutoff modulation
    SinOsc lfo_Q;          // lfo for Q modulation

    // IIRFilters
    juce::IIRFilter filter;

    // Other parameters, sample rate and original frequency of oscillators
    float sampleRate;
    float freq {0.0f};            // original frequency
}; 