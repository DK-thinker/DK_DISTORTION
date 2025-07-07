/*
  ==============================================================================

   This file holds the main DSP of the plug in, including block processing.

   Summary of DSP and Parameters:
   Drive - Amount of drive to apply to input signal

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <memory>
//=============================================================================
// DSP Helpers
static float getSign(float x){
    if (x < 0.0) return -1.0;
    if (0.0 <= x) return 1.0;
}

/* 
 * The way folding works; 
 * We want this sort of recursive folding for when we apply extreme gain,
 * the way to do this is by working with the absolut value, following this 
 * folding down if greater than 1 and folding up if less than 0 until the sample
 * is 0 < x < 1. Then we add the sign back in and return.
 * @TODO add rectifier mode, a binaray paramater that toggles adding the sign
 * back
 * */
static float foldSample(float x, float drive){
    float sign = getSign(x);
    x = std::abs(x);
    x *= drive;
    while (!(0.0f < x && x < 1.0f)){
        if (x < 0.0f){
            x = - x;
        }
        else if (1.0f < x){
            x = 2.0f - x;
        }
    }
    return sign * x;
}

//==============================================================================
// for the parameters constructor, since ther is no undo functionality, the 2nd 
// arg is a nullptr
DK_DISTORTIONAudioProcessor::DK_DISTORTIONAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
    parameters (*this, nullptr, "PARAMETERS", createParameterLayout())
#endif
{
    // Want to initilize the parameter pointers here

}

DK_DISTORTIONAudioProcessor::~DK_DISTORTIONAudioProcessor()
{
}

//==============================================================================
const juce::String DK_DISTORTIONAudioProcessor::getName() const
{
    return "DK_DISTORTION";
}

bool DK_DISTORTIONAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool DK_DISTORTIONAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool DK_DISTORTIONAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double DK_DISTORTIONAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int DK_DISTORTIONAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int DK_DISTORTIONAudioProcessor::getCurrentProgram()
{
    return 0;
}

void DK_DISTORTIONAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String DK_DISTORTIONAudioProcessor::getProgramName (int index)
{
    return {};
}

void DK_DISTORTIONAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void DK_DISTORTIONAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    DBG("Prepare to Play");
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void DK_DISTORTIONAudioProcessor::releaseResources()
{
    DBG("Release Resources");
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool DK_DISTORTIONAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void DK_DISTORTIONAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    float drive = 10.0;

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
    int i = 0;
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);
        for (int sample = 0; sample < buffer.getNumSamples(); sample++){
            channelData[sample] = foldSample(channelData[sample], drive);
        }
        if (i % 10000 == 0){
            DBG("ProcessBlock");
        }
        i++;

        // ..do something to the data...
    }
}

//==============================================================================
bool DK_DISTORTIONAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* DK_DISTORTIONAudioProcessor::createEditor()
{
    return new NewProjectAudioProcessorEditor (*this);
}

//==============================================================================
void DK_DISTORTIONAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void DK_DISTORTIONAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

/*
 * @brief paramater constructor, follows the juce apvt format
 * What each (plugin) parameter does 
 * drive - The amount to multiply the normalised input by
 * rect - if true, evey sample is an absolute value
 * dryWet - standard dry wet, 0.0 is full dry 1.0 is full wet
 * output - gain (in dB) with a meter in editor.
 */
juce::AudioProcessorValueTreeState::ParameterLayout 
DK_DISTORTIONAudioProcessor::createParameterLayout(){

    AudioProcessorValueTreeState::ParameterLayout layout;

    layout.add(std::make_unique<AudioParameterFloat> 
        (
            "drive", 
            "Drive Amount",
            1.0f, //min value
            10.0f, //max
            1.5f //default
        )
    );
    layout.add(std::make_unique<AudioParameterBool>
        (
            "rect",
            "Rectify",
            false //default
        )
    );
    layout.add(std::make_unique<AudioParameterFloat>
        (
            "dryWet",
            "Dry/Wet",
            0.0f, //min
            1.0f, //max
            0.8f //default
        )
    );
    layout.add(std::make_unique<AudioParameterFloat>
        (
            "output",
            "Output Gain dB",
            -60.0f, //min
            6.0f, //max
            0.0f //default
        )
    );
    return layout;
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new DK_DISTORTIONAudioProcessor();
}
