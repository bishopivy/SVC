/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SVCAudioProcessorEditor::SVCAudioProcessorEditor (SVCAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (500, 530);

    // Add your audio processor editor
    addAndMakeVisible(processorEditor);

    // Create and add a text label
    addAndMakeVisible(textLabel);
    textLabel.setText("Bishop-Transfomer: Convert any vocals to Bishop's voice", juce::dontSendNotification);
    textLabel.setJustificationType(juce::Justification::centred);
    
    addAndMakeVisible(instr_text);
    juce::String ctrls = "Controls";
    instr_text.setText(ctrls, juce::dontSendNotification);
    instr_text.setJustificationType(juce::Justification::centred);
    
    addAndMakeVisible(transfer_text);
    juce::String transfer_str = "Transfer: enable, then playback desired input section, then disable.\n";
    transfer_text.setText(transfer_str, juce::dontSendNotification);
    
    addAndMakeVisible(pitch_text);
    juce::String pitch_str = "Match Pitch: enable to preserve melody, disable for speaking vocals.\n";
    pitch_text.setText(pitch_str, juce::dontSendNotification);
    
    addAndMakeVisible(trouble);
    juce::String t_str = "Trouble Shooting";
    trouble.setText(t_str, juce::dontSendNotification);
    trouble.setJustificationType(juce::Justification::centred);
    
    addAndMakeVisible(trouble1);
    juce::String t1_str = "Pitch jumping eradically: SVC is struggling to find the correct pitch, try a cleaner signal.\n";
    trouble1.setText(t1_str, juce::dontSendNotification);
    
    addAndMakeVisible(trouble2);
    juce::String t2_str = "Missing sections: SVC filters out quiet sections, try turning up gain and re-transfering.\n";
    trouble2.setText(t2_str, juce::dontSendNotification);
    
    addAndMakeVisible(notes);
    juce::String notes_str = "Notes";
    notes.setText(notes_str, juce::dontSendNotification);
    notes.setJustificationType(juce::Justification::centred);
    
    addAndMakeVisible(notes1);
    juce::String notes1_str = "The model only works with isolated vocals with no reverb or effects.\n";
    notes1.setText(notes1_str, juce::dontSendNotification);
    
    addAndMakeVisible(notes2);
    juce::String notes2_str = "Performance may suffer for vocals with no obvious pitch.\n";
    notes2.setText(notes2_str, juce::dontSendNotification);
    
    addAndMakeVisible(notes3);
    juce::String notes3_str = "Model was trainined in English language, so non-English input may be lower quality.\n";
    notes3.setText(notes3_str, juce::dontSendNotification);
    
}

SVCAudioProcessorEditor::~SVCAudioProcessorEditor()
{
}

//==============================================================================
void SVCAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
}

void SVCAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    // Set the bounds for your audio processor editor
    processorEditor.setBounds(150, 0, getWidth(), getHeight());
    int top_margin = 60;
    int line_height = 50;

    // Set the bounds for the text label (adjust as needed)
    textLabel.setBounds(20, 20 + top_margin, getWidth() - 40, 30);
    instr_text.setBounds(20, 20 + top_margin + line_height, getWidth() - 40, 30);
    transfer_text.setBounds(20, 20 + top_margin + 1.5 * line_height, getWidth() - 40, 30);
    pitch_text.setBounds(20, 20 + top_margin + 2.0 * line_height, getWidth() - 40, 30);
    trouble.setBounds(20, 20 + top_margin + 3.0 * line_height, getWidth() - 40, 30);
    trouble1.setBounds(20, 20 + top_margin + 3.7 * line_height, getWidth() - 40, 30);
    trouble2.setBounds(20, 20 + top_margin + 4.7 * line_height, getWidth() - 40, 30);
    notes.setBounds(20, 20 + top_margin + 5.7* line_height, getWidth() - 40, 30);
    notes1.setBounds(20, 20 + top_margin + 6.3 * line_height, getWidth() - 40, 30);
    notes2.setBounds(20, 20 + top_margin + 6.8 * line_height, getWidth() - 40, 30);
    notes3.setBounds(20, 20 + top_margin + 7.5 * line_height, getWidth() - 40, 30);
}
