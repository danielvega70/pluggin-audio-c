#include "PluginProcessor.h"
#include "PluginEditor.h"

// Constructor for the plugin processor
AudioCleaningProcessor::AudioCleaningProcessor()
{
    // Set the number of input and output channels
    setPlayConfigDetails(0, 2, 44100, 512);
}

// Destructor for the plugin processor
AudioCleaningProcessor::~AudioCleaningProcessor() {}

// Process a block of audio samples
void AudioCleaningProcessor::processBlock(AudioSampleBuffer& buffer, MidiBuffer&)
{
    // Get the number of channels in the buffer
    int numChannels = buffer.getNumChannels();
    
    // Loop over each channel
    for (int channel = 0; channel < numChannels; ++channel)
    {
        // Get a pointer to the channel data
        float* channelData = buffer.getWritePointer(channel);
        
        // Create an FFT object to perform the spectrum analysis
        FFT fft;
        
        // Set the FFT size to the buffer size
        fft.init(buffer.getNumSamples());
        
        // Perform the FFT on the channel data
        fft.performRealOnlyForwardTransform(channelData);
        
        // Get the magnitude of the frequency bins
        float* magnitudes = fft.getMagnitude(false);
        
        // Loop over each frequency bin and perform noise reduction
        for (int i = 0; i < fft.getSize() / 2; ++i)
        {
            // Apply a noise reduction threshold
            if (magnitudes[i] < threshold) {
                magnitudes[i] = 0;
            }
        }
        
        // Apply a de-esser to the channel data
        DeEsser deEsser;
        deEsser.process(channelData, buffer.getNumSamples(), sampleRate);
        
        // Apply EQ to the channel data
        EQ eq;
        eq.process(channelData, buffer.getNumSamples(), sampleRate);
        
        // Apply dynamic range compression to the channel data
        DynamicRangeCompressor compressor;
        compressor.process(channelData, buffer.getNumSamples(), sampleRate);
    }
}

// Create the editor for the plugin
AudioProcessorEditor* AudioCleaningProcessor::createEditor()
{
    return new AudioCleaningProcessorEditor(*this);
}
