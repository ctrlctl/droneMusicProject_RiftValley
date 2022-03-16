/*
  ==============================================================================

    FMSynth.h
    Created: 14 Mar 2022 10:07:26am
    Author:  lancelotli

  ==============================================================================
*/

#pragma once
#include "Oscillators.h"
#include <JuceHeader.h>

class FMSynth 
{
public:
    
    void setSampleRate(float sr) 
    {
        sampleRate = sr;
        sinOsc.setSampleRate(sr);
        triOsc.setSampleRate(sr);
        sawOsc.setSampleRate(sr);
        squareOsc.setSampleRate(sr);
        lfoTri.setSampleRate(sr);
        lfoSin.setSampleRate(sr);
        lfoSaw.setSampleRate(sr);
        lfoSaw.setFrequency(0.01f);
        lfoTri.setFrequency(0.01f);
    }

    // process FM sine wave using parameters midiNoteNumber and fmDepth
    float processFMSin(int midiNoteNumber, float depth)
    {
        fmDepth = depth;
        lfoSin.setFrequency((lfoTri.process() + 1) / 30); // 2
        fmMod = lfoTri.process() * fmDepth;
        sinOsc.setFrequency(juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber) + fmMod);
        //sinOsc.setPhase(0.5f);
        float oscOut = sinOsc.process();
        return oscOut;
    }

    // process FM triangle wave using parameters midiNoteNumber and fmDepth
    float processFMTri(int midiNoteNumber, float depth)
    {
        srand(time(NULL));
        lfoSin.setFrequency((lfoTri.process() + 1) / 30);  // 3
        fmDepth = depth + lfoSin.process() * 50;
        fmMod = lfoTri.process() * fmDepth ; 
        float gain = lfoSin.process() + 1.5;
        triOsc.setFrequency(juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber + ((rand() % 700)-400) / 100) + fmMod);
        //triOsc.setPhase(0.5f);
        float oscOut = gain * triOsc.process();
        return oscOut;
    }

    // process FM saw wave using parameters midiNoteNumber and fmDepth
    float processFMSaw(int midiNoteNumber, float depth)
    {
        lfoSin.setFrequency((lfoTri.process() + 1) / 30); // 3
        fmDepth = depth + lfoSin.process() * 20;
        fmMod = lfoTri.process() * fmDepth;
        sawOsc.setFrequency(juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber) + fmMod);
        //sawOsc.setPhase(0.5f);
        float oscOut = sawOsc.process();
        return oscOut;
    }
     
    // process FM square wave using parameters midiNoteNumber and fmDepth
    float processFMSquare(int midiNoteNumber, float depth)
    {

        fmDepth = depth;
        lfoSin.setFrequency(0.01f);     
        float lfoSin_Val = lfoSin.process();
        fmMod = lfoSin_Val * fmDepth; //
        squareOsc.setFrequency(juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber)   + fmMod);  //
        squareOsc.setPulseWidth(lfoSin.process() / 2 + 0.5f);
        //squareOsc.setPhase(0.5f);
        float oscOut = squareOsc.process();
        return oscOut;
    }

private:
    // Frequency modulated Osc
    SinOsc sinOsc;
    TriOsc triOsc;
    SawOsc sawOsc;
    SquareOsc squareOsc;

    // lfo for frequency modulation
    TriOsc lfoTri;
    SinOsc lfoSin;
    SawOsc lfoSaw;

    // FM parameters
    float fmMod{ 0.0f };
    float fmDepth{ 0.0f };
    float sampleRate;
    int flag = 0;

};