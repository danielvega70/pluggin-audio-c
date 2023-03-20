#include "PluginProcessor.h"
#include "PluginEditor.h"

// Constructor for the plugin editor
AudioCleaningProcessorEditor::AudioCleaningProcessorEditor(AudioCleaningProcessor& processor)
    : AudioProcessorEditor(&processor), processor(processor)
{
    // Set the size of the plugin window
    setSize(800, 600);
    
    // Create the spectrum display component
    spectrumDisplay = new SpectrumDisplayComponent(processor.getFFTSize(), processor.getSampleRate());
    addAndMakeVisible(spectrumDisplay);
    
    // Create the EQ display component
    eqDisplay = new EQDisplayComponent(processor.getEQSettings());
    addAndMakeVisible(eqDisplay);
    
    // Create the compressor display component
    compressorDisplay = new CompressorDisplayComponent(processor.getCompressorSettings());
    addAndMakeVisible(compressorDisplay);
    
    // Create the controls for the EQ settings
    eqControls = new EQControlsComponent(processor.getEQSettings());
    addAndMakeVisible(eqControls);
    
    // Create the controls for the compressor settings
    compressorControls = new CompressorControlsComponent(processor.getCompressorSettings());
    addAndMakeVisible(compressorControls);
}

// Destructor for the plugin editor
AudioCleaningProcessorEditor::~AudioCleaningProcessorEditor() {}

// Paint the plugin editor
void AudioCleaningProcessorEditor::paint(Graphics& g)
{
    // Set the background color of the plugin window
    g.fillAll(Colours::darkgrey);
}

// Resize the plugin editor
void AudioCleaningProcessorEditor::resized()
{
    // Set the size and position of the spectrum display component
    spectrumDisplay->setBounds(10, 10, getWidth() - 20, 100);
    
    // Set the size and position of the EQ display component
    eqDisplay->setBounds(10, 120, getWidth() - 20, 100);
    
    // Set the size and position of the compressor display component
    compressorDisplay->setBounds(10, 230, getWidth() - 20, 100);
    
    // Set the size and position of the EQ controls component
    eqControls->setBounds(10, 340, getWidth() - 20, 200);
    
    // Set the size and position of the compressor controls component
    compressorControls->setBounds(10, 550, getWidth() - 20, 200);
}

// Constructor for the spectrum display component
SpectrumDisplayComponent::SpectrumDisplayComponent(int fftSize, double sampleRate)
    : fftSize(fftSize), sampleRate(sampleRate)
{
    // Set the number of frequency bands to display
    numBands = fftSize / 2;
    
    // Create an array to store the frequency band values
    bands.resize(numBands);
}

// Destructor for the spectrum display component
SpectrumDisplayComponent::~SpectrumDisplayComponent() {}

// Paint the spectrum display component
void SpectrumDisplayComponent::paint(Graphics& g)
{
    // Set the color of the spectrum display
    g.setColour(Colours::white);
    
    // Get the width and height of the component
    int width = getWidth();
    int height = getHeight();
    
    // Draw the spectrum display
    for (int i = 0; i < numBands; ++i)
    {
        // Calculate the x-coordinate of the frequency band
        float x = i * width / (float)numBands;
