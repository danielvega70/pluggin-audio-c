#include "../JuceLibraryCode/JuceHeader.h"

class CompressorPluginAudioProcessor  : public AudioProcessor
{
public:
    CompressorPluginAudioProcessor()
    {
        // Define the plugin parameters
        addParameter(thresholdParam = new AudioParameterFloat("threshold", "Threshold", 0.0f, 1.0f, 0.5f));
        addParameter(ratioParam = new AudioParameterFloat("ratio", "Ratio", 1.0f, 10.0f, 2.0f));
        addParameter(attackParam = new AudioParameterFloat("attack", "Attack", 0.0f, 1.0f, 0.1f));
        addParameter(releaseParam = new AudioParameterFloat("release", "Release", 0.0f, 1.0f, 0.5f));
        addParameter(gainParam = new AudioParameterFloat("gain", "Gain", -12.0f, 12.0f, 0.0f));
        
        // Initialize the state variables
        envelope = 0.0f;
        thresholdLin = 0.0f;
        gainLin = 0.0f;
        slope = 1.0f / *ratioParam;
        attackCoeff = exp(-1.0f / (0.001f * getSampleRate() * *attackParam));
        releaseCoeff = exp(-1.0f / (0.001f * getSampleRate() * *releaseParam));
    }

    ~CompressorPluginAudioProcessor() {}

    void prepareToPlay (double sampleRate, int samplesPerBlock) override {}
    void releaseResources() override {}
    void processBlock (AudioBuffer<float>& buffer, MidiBuffer&) override
    {
        const int numChannels = buffer.getNumChannels();
        const int numSamples = buffer.getNumSamples();
        const float gaindB = *gainParam;
        gainLin = pow(10.0f, 0.05f * gaindB);
        thresholdLin = pow(10.0f, -20.0f + 40.0f * *thresholdParam);

        for (int ch = 0; ch < numChannels; ++ch) {
            auto* channelData = buffer.getWritePointer(ch);

            for (int i = 0; i < numSamples; ++i) {
                float x = channelData[i];
                float envTarget = fabs(x);
                if (envTarget > envelope) {
                    envelope = attackCoeff * (envelope - envTarget) + envTarget;
                } else {
                    envelope = releaseCoeff * (envelope - envTarget) + envTarget;
                }
                float gainReduction = 1.0f;
                if (envelope > thresholdLin) {
                    float excess = envelope / thresholdLin - 1.0f;
                    gainReduction = 1.0f + slope * excess;
                    gainReduction = jmax(gainReduction, 0.0f);
                    gainReduction = jmin(gainReduction, 1.0f);
                }
                x *= gainReduction;
                x *= gainLin;
                channelData[i] = x;
            }
        }
    }

    AudioProcessorEditor* createEditor() override { return nullptr; }
    bool hasEditor() const override { return false; }

    const String getName() const override { return "Compressor Plugin"; }
    bool acceptsMidi() const override { return false; }
    bool producesMidi() const
