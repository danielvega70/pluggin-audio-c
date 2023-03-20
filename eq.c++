class EQAudioProcessor  : public AudioProcessor
{
public:
    EQAudioProcessor()
    {
        // Create the parameters
        addParameter (frequencyParam = new AudioParameterFloat ("frequency", "Frequency", 20.0f, 20000.0f, 1000.0f));
        addParameter (gainParam = new AudioParameterFloat ("gain", "Gain", -24.0f, 24.0f, 0.0f));
        addParameter (qParam = new AudioParameterFloat ("q", "Q", 0.1f, 10.0f, 1.0f));
        
        // Set the filter coefficients
        updateFilter();
    }

    ~EQAudioProcessor() {}

    void prepareToPlay (double sampleRate, int samplesPerBlock) override
    {
        // Set the sample rate and block size
        filter.setSampleRate (sampleRate);
        buffer.setSize (getNumInputChannels(), samplesPerBlock);
    }

    void releaseResources() override {}

    void processBlock (AudioSampleBuffer& buffer, MidiBuffer&) override
    {
        int totalNumInputChannels = getTotalNumInputChannels();
        int totalNumOutputChannels = getTotalNumOutputChannels();

        // Process each channel
        for (int channel = 0; channel < totalNumInputChannels; ++channel)
        {
            // Get the input and output buffers
            auto* channelData = buffer.getWritePointer (channel);
            auto* outputData = buffer.getWritePointer (channel);
            
            // Apply the filter to each sample
            for (int i = 0; i < buffer.getNumSamples(); ++i)
            {
                outputData[i] = filter.processSingleSampleRaw (channelData[i]);
            }
        }
    }

    AudioProcessorEditor* createEditor() override
    {
        return new EQAudioProcessorEditor (*this);
    }

    bool hasEditor() const override { return true; }

    const String getName() const override { return "EQ"; }

    bool acceptsMidi() const override { return false; }
    bool producesMidi() const override { return false; }

    double getTailLengthSeconds() const override { return 0.0; }

    int getNumPrograms() override { return 1; }
    int getCurrentProgram() override { return 0; }
    void setCurrentProgram (int) override {}
    const String getProgramName (int) override { return {}; }
    void changeProgramName (int, const String&) override {}

    void getStateInformation (MemoryBlock& destData) override {}
    void setStateInformation (const void* data, int sizeInBytes) override {}

private:
    AudioParameterFloat* frequencyParam;
    AudioParameterFloat* gainParam;
    AudioParameterFloat* qParam;
    BiquadFilter filter;
    AudioSampleBuffer buffer;

    void updateFilter()
    {
        float frequency = *frequencyParam;
        float gain = Decibels::decibelsToGain (*gainParam);
        float q = *qParam;
        filter.setCoefficients (BiquadCoefficients::makePeakFilter (filter.getSampleRate(), frequency, q, gain));
    }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EQAudioProcessor)
};
