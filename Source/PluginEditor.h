/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class SVCAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    SVCAudioProcessorEditor (SVCAudioProcessor&);
    ~SVCAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    SVCAudioProcessor& audioProcessor;
    juce::GenericAudioProcessorEditor processorEditor{processor};
    juce::Label textLabel;
    juce::Label instr_text;
    juce::Label transfer_text;
    juce::Label pitch_text;
    juce::Label trouble;
    juce::Label trouble1;
    juce::Label trouble2;
    juce::Label notes;
    juce::Label notes1;
    juce::Label notes2;
    juce::Label notes3;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SVCAudioProcessorEditor)
};
