/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <stdlib.h>
#include <iostream>
#include <fstream>

//std::ofstream outputFile("/Users/bishopcrowley/Music/out.txt");

//==============================================================================
SVCAudioProcessor::SVCAudioProcessor()
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

SVCAudioProcessor::~SVCAudioProcessor()
{
}

//==============================================================================
const juce::String SVCAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool SVCAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool SVCAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool SVCAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double SVCAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int SVCAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int SVCAudioProcessor::getCurrentProgram()
{
    return 0;
}

void SVCAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String SVCAudioProcessor::getProgramName (int index)
{
    return {};
}

void SVCAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void SVCAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
//    std::ofstream outputFile("/Users/bishopcrowley/Music/out.txt");
    // Get the current time point
    auto currentTimePoint = std::chrono::system_clock::now();

    // Convert the time point to a time_t object
    std::time_t currentTime = std::chrono::system_clock::to_time_t(currentTimePoint);

    // Convert the time_t object to a local time structure
    std::tm* localTime = std::localtime(&currentTime);

    // Format and print the local time
    char timeString[100];  // Buffer to hold the formatted time string
    std::strftime(timeString, sizeof(timeString), "%Y-%m-%d %H:%M:%S", localTime);

    int init_size = 10 * sampleRate;
    rec_buffer.setSize(getTotalNumOutputChannels(), init_size, false, true);
    processing = false;
    converted = false;
    start_point = 0;

    juce::File currentDirectory = juce::File::getCurrentWorkingDirectory();

    juce::String current_path = juce::SystemStats::getEnvironmentVariable("PATH", "couldn't find");
    juce::String new_path = current_path + ":" + "~/Library/Python/3.9/bin";
    setenv("PATH", new_path.toRawUTF8(), 1);
}

void SVCAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool SVCAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void SVCAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
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

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
//    for (int channel = 0; channel < totalNumInputChannels; ++channel)
//    {
//        auto* channelData = buffer.getWritePointer (channel);
//
//        // ..do something to the data...
//    }
    // Get the current playhead position

    bool transfer = apvts.getRawParameterValue("Transfer")->load();
    bool match_pitch = apvts.getRawParameterValue("Match Pitch")->load();
    bool copying = false;
    juce::int64 playheadTimeInSamples = 0;
    int playhead_int = 0;
    
    
    if (auto* playHead = getPlayHead())
    {
        juce::Optional<juce::int64> possible_time = playHead->getPosition()->getTimeInSamples();
        if (possible_time.hasValue()) {
            copying = true;
            playheadTimeInSamples = *possible_time;
            playhead_int = static_cast<int>(playheadTimeInSamples);
        }
    }
    double time_in_seconds = playhead_int / getSampleRate();
    if (time_in_seconds - static_cast<float>(static_cast<int>(time_in_seconds)) < 0.01) {
        juce::Logger::outputDebugString(std::to_string(static_cast<int>(time_in_seconds)));
    }
           
    if (transfer)
    {
        processing = true;
//        juce::Logger::outputDebugString(std::to_string(std::round(playhead_int / getSampleRate())));
       if (playheadTimeInSamples + buffer.getNumSamples() >= rec_buffer.getNumSamples())
           {
               resizeBuffer(playhead_int);
           }
       // Append incoming audio to the recording buffer
       for (int channel = 0; channel < 2; channel++){
           rec_buffer.copyFrom(channel, playhead_int, buffer, channel, 0, buffer.getNumSamples());
       }
    } else {
        juce::String file_name = "~/Music/svc_test";
        juce::String file_path_juce = file_name + ".wav";
        juce::String converted_path = file_name + ".out.wav";
        if (processing) {
            const char *file_path = file_path_juce.toUTF8();
            juce::String rm_command = "rm " + file_path_juce;
            system(rm_command.toUTF8());
            juce::String rm_command2 = "rm " + converted_path;
            system(rm_command2.toUTF8());
//            outputFile << "about to write" << std::endl;
            
            juce::FileOutputStream file = juce::FileOutputStream(juce::File(file_path), playhead_int);
            juce::WavAudioFormat wavFormat;
            auto writer = wavFormat.createWriterFor(&file,
                                                    getSampleRate(),
                                                    2, //channels
                                                    16, //bit depth
                                                    {}, //meta data
                                                    0); //idk
            
            writer->writeFromAudioSampleBuffer(rec_buffer, 0, rec_buffer.getNumSamples());
            writer->flush();
//            outputFile << "just wrote" << std::endl;
            juce::Logger::outputDebugString("just wrote");
            
            juce::String model_path = "~/Music/code_stuff/so-vits-svc-fork/models/bishop2.pth";
            juce::String config_path = "~/Music/code_stuff/so-vits-svc-fork/notebooks/logs/44k/config.json";
            juce::String command = "svc infer " + file_path_juce + " -m " + model_path + " -c " + config_path + " -d cpu";
            if (match_pitch) {
                command += " -na";
            }
            system(command.toUTF8());
//            system("python3 /Users/bishopcrowley/Music/code_stuff/juceprojs/SVC/Source/svc.py");
//            outputFile << "just converted" << std::endl;
            juce::Logger::outputDebugString("just converted");
//            outputFile << std::to_string(covert_res) << std::endl;
            
            processing = false;
            converted = true;
            juce::Logger::outputDebugString(std::to_string(playhead_int / getSampleRate()));
            rec_buffer.clear();
            juce::Logger::outputDebugString("cleared buffer");
        }
        if (converted) {
            if (juce::File(converted_path).exists()) {
                juce::FileInputStream file = juce::FileInputStream(juce::File(converted_path));
                juce::WavAudioFormat wavFormat;
                
                auto reader = wavFormat.createReaderFor(&file, false);

                if (reader != nullptr && (playhead_int - reader->lengthInSamples) < reader->lengthInSamples && playhead_int > reader->lengthInSamples) {
                    reader->read(&buffer, 0, buffer.getNumSamples(), playhead_int - reader->lengthInSamples, true, true);
//                    outputFile << "read file" << std::endl;
                } else {
//                    outputFile << "file out of range" << std::endl;
                }
            } else {
//                outputFile << "file doesn't exist" << std::endl;
            }
           
        }
    }
    if (!copying) {
//        juce::Logger::outputDebugString("not copying");
    }
    
}
void SVCAudioProcessor::resizeBuffer(int playhead_int)
{
    // Create a temporary buffer with the new size
    juce::AudioBuffer<float> newBuffer(rec_buffer.getNumChannels(), 1.5 * playhead_int);
    
    // Copy the existing data to the new buffer
    //        newBuffer.copyFrom(rec_buffer, 0, 0, rec_buffer.getNumSamples());
    for (int channel = 0; channel < 2; channel += 1) {
        
        newBuffer.copyFrom(channel, 0, rec_buffer, channel, 0, rec_buffer.getNumSamples());
        
        // Replace the existing buffer with the new one
        rec_buffer = newBuffer;
    }
    juce::Logger::outputDebugString("resized");
}

//==============================================================================
bool SVCAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* SVCAudioProcessor::createEditor()
{
    return new SVCAudioProcessorEditor (*this);
//    return new juce::GenericAudioProcessorEditor(*this);
}

//==============================================================================
void SVCAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void SVCAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

juce::AudioProcessorValueTreeState::ParameterLayout SVCAudioProcessor::createParameterLayout() {
    juce::AudioProcessorValueTreeState::ParameterLayout layout;
    
    layout.add(std::make_unique<juce::AudioParameterBool>(juce::ParameterID("Transfer", 1), "Transfer", false));
    layout.add(std::make_unique<juce::AudioParameterBool>(juce::ParameterID("Match Pitch", 1), "Match Pitch", true));
    return layout;
}
//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SVCAudioProcessor();
}
